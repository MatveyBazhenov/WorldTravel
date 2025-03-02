#include "login.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/server/http/http_status.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"

#include <fmt/format.h>

#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

namespace service_template {

namespace {

class LoginHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-login";

  LoginHandler(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context), 
      postgres_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {


    const auto& json = userver::formats::json::FromString(request.RequestBody());
    auto login = json["login"].As<std::string>();

    constexpr const char* kCheckLoginQuery = R"(
        SELECT COUNT(*)
        FROM hello_schema.users
        WHERE name = $1
    )";

    auto result = postgres_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kCheckLoginQuery, login);
    auto count = result.AsSingleRow<int>();
    if (count > 0) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
        return service_template::LoginResponse(login, Loginstatus::kExists);
    }
    if (login.size() <= 50) {
        return service_template::LoginResponse(login, Loginstatus::kSuccess);
    } else {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return service_template::LoginResponse(login, Loginstatus::kInvalid);
    }
  }

  userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace

std::string LoginResponse(std::string_view login, Loginstatus status) {
  switch (status) {
    case service_template::Loginstatus::kSuccess:
        return fmt::format("Welcome, {}", login);
    case service_template::Loginstatus::kInvalid:
        return "Invalid login. Too long";
    case service_template::Loginstatus::kExists:
        return "Login already exists. Please choose a differnet one";
    default:
        return "Unknown Status";
  } 

  UASSERT(false);
}

void AppendLogin(userver::components::ComponentList& component_list) {
  component_list.Append<LoginHandler>();
  component_list.Append<userver::components::Postgres>("postgres-db-1");
  component_list.Append<userver::clients::dns::Component>();
}

}  // namespace service_template
