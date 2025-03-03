#include "registration.hpp"

#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster_types.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>

#include <fmt/format.h>
#include <string>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include <userver/formats/json/serialize.hpp>

namespace service_template {

namespace {

class RegistrationHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-registration";

  RegistrationHandler(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        postgres_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    
    const auto& json = userver::formats::json::FromString(request.RequestBody());
    auto username = json["login"].As<std::string>();

    if (username.empty() || username.size() > 50) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return RegistrationResponse(username, RegistrationStatus::kInvalid);
    }

    constexpr const char* kCheckUserQuery = R"(
        SELECT COUNT(*) FROM hello_schema.users WHERE name = $1
    )";
    auto result = postgres_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kCheckUserQuery, username);
    auto count = result.AsSingleRow<int>();

    if (count > 0) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
        return RegistrationResponse(username, RegistrationStatus::kExists);
    }

    constexpr const char* kInsertUserQuery = R"(
        INSERT INTO hello_schema.users (name) VALUES ($1)
    )";
    postgres_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kInsertUserQuery, username);

    return RegistrationResponse(username, RegistrationStatus::kSuccess);
  }

  userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace

std::string RegistrationResponse(std::string_view username, RegistrationStatus status) {
  switch (status) {
    case RegistrationStatus::kSuccess:
        return fmt::format("User {} successfully registered.\n", username);
    case RegistrationStatus::kExists:
        return "User already exists. Please choose a different username.\n";
    case RegistrationStatus::kInvalid:
        return "Invalid username. Must be non-empty and up to 50 characters.\n";
    default:
        return "Unknown registration status.\n";
  }
}

void AppendRegistration(userver::components::ComponentList& component_list) {
  component_list.Append<RegistrationHandler>();
}

}  // namespace service_template
