#pragma once

#include <userver/components/component_context.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/clients/http/request.hpp>
#include <userver/clients/http/response.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/formats/json.hpp>
#include <userver/utils/assert.hpp>

#include "aviasalesAPI.hpp"

namespace my_service {

class HttpClientComponent final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "http-client-component";

  HttpClientComponent(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
      http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()),
      aviasales_api_(&context.FindComponent<AviasalesAPI>()) {}
      
      
  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
  std::string response = aviasales_api_->GetMyIP();
  return response;
  }

 private:
 userver::clients::http::Client* http_client_;
  AviasalesAPI* aviasales_api_;
};

}  // namespace my_service
