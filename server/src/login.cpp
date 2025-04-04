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

namespace service_template { // in login there is a user already!! it is not a registration

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
      auto password = json["password"].As<std::string>();
    
      if (login.empty() || login.size() >= 50 || password.empty() ||
          password.size() > 50) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return LoginResponse(login, LoginStatus::kInvalid);
      }
    
      constexpr const char* kCheckUserQuery = R"(
        SELECT password, user_key FROM WorldTravel.users WHERE username = $1
      )";
    
      auto result = postgres_->Execute(
          userver::storages::postgres::ClusterHostType::kMaster,
          kCheckUserQuery,
          login
      );
    
      if (result.IsEmpty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return LoginResponse(login, LoginStatus::kNotFound);
      }
    
      auto row = result.Front();
      auto stored_password = row["password"].As<std::string>();
      auto user_key = row["user_key"].As<std::string>();
    
      if (stored_password != password) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
        return LoginResponse(login, LoginStatus::kWrongPassword);
      }
    
      return LoginResponse(user_key, LoginStatus::kSuccess);
    }

userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace

std::string LoginResponse(std::string_view user_key, LoginStatus status) {
switch (status) {
  case LoginStatus::kSuccess:
      return fmt::format(R"({{"status": "ok", "user_key":"{}"}})", user_key);
  case LoginStatus::kWrongPassword:
      return R"({"status": "error", "message": "Incorrect password."})"
             "\n";
  case LoginStatus::kNotFound:
      return R"({"status": "error", "message": "Unknown user."})"
             "\n";
  case LoginStatus::kInvalid:
      return R"({"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"})"
             "\n";
  default:
    return R"({"status": "error", "message": "Unknown status"})"
            "\n";
}
}

void AppendLogin(userver::components::ComponentList& component_list) {
component_list.Append<LoginHandler>();
}

}  // namespace service_template