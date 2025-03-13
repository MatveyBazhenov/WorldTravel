#include "registration.hpp"

#include <string_view>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster_types.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>

#include <fmt/format.h>
#include <string>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/uuid7.hpp>

namespace service_template {

namespace {

class RegistrationHandler final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-registration";

  RegistrationHandler(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        postgres_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto& json =
        userver::formats::json::FromString(request.RequestBody());

    auto username = json["username"].As<std::string>();
    auto password = json["password"].As<std::string>();
    std::string user_key = userver::utils::generators::GenerateUuidV7();

    if (username.empty() || username.size() >= 50 || password.empty() ||
        password.size() > 50) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
      return RegistrationResponse(username, RegistrationStatus::kInvalid);
    }

    constexpr const char* kCheckUserQuery = R"(
        SELECT COUNT(*) FROM WorldTravel.users WHERE username = $1
    )";
    auto result = postgres_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster, kCheckUserQuery,
        username);
    auto count = result.AsSingleRow<int>();

    if (count > 0) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
      return RegistrationResponse(username, RegistrationStatus::kConflict);
    }

    constexpr const char* kInsertUserQuery = R"(
        INSERT INTO WorldTravel.users (username, password, user_key) VALUES ($1, $2, $3)
    )";
    postgres_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       kInsertUserQuery, username, password, user_key);

    return RegistrationResponse(user_key, RegistrationStatus::kSuccess);
  }

  userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace

std::string RegistrationResponse(std::string_view message,
                                 RegistrationStatus status) {
  switch (status) {
    case RegistrationStatus::kSuccess:
      return fmt::format(R"({{"status": "ok", "user_key": "{}"}})"
                         "\n",
                         message);
    case RegistrationStatus::kConflict:
      return fmt::format(
          R"({{"status": "error", "message": "User {} already exists"}})"
          "\n",
          message);
    case RegistrationStatus::kInvalid:
      return R"({"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"})"
             "\n";
    default:
      return R"({"status": "error", "message": "Unknown registration status"})"
             "\n";
  }
}

void AppendRegistration(userver::components::ComponentList& component_list) {
  component_list.Append<RegistrationHandler>();
}

}  // namespace service_template
