#pragma once

#include <string>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/clients/http/request.hpp>
#include <userver/clients/http/response.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/component_list.hpp>
#include <userver/formats/json.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utils/assert.hpp>
#include <utility>

#include "aviasalesAPI.hpp"
#include "chatgptAPI.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/utils/fmt_compat.hpp"

namespace my_service {

class HttpClientComponent final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "http-client-component";

  HttpClientComponent(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        http_client_(&context.FindComponent<userver::components::HttpClient>()
                          .GetHttpClient()),
        aviasales_api_(&context.FindComponent<AviasalesAPI>()),
        chatgpt_api_(&context.FindComponent<ChatGPTAPI>()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const userver::formats::json::Value& json =
        userver::formats::json::FromString(request.RequestBody());

    userver::formats::json::Value tickets;
    tickets = aviasales_api_->GetTickets(json);

    std::vector<Attraction> description;
    description = chatgpt_api_->GetTravelInfo(json);
    
    userver::formats::json::ValueBuilder route;
    if (tickets["success"].As<bool>()) {
      route["origin_city"] = json["origin"];
      route["destination_city"] = json["destination"];
      route["origin_IATA"] = tickets["data"][0]["origin"];
      route["destination_IATA"] = tickets["data"][0]["destination"];
      route["departure_at"] = tickets["data"][0]["departure_at"];
      route["price"] = tickets["data"][0]["price"];
      for(const Attraction& sight: description){
        route["description"][sight.name] = sight.description;
      }
    } else {
      return "There were problems processing your request\n";
    }
    return userver::formats::json::ToString(route.ExtractValue());
  }

 private:
  userver::clients::http::Client* http_client_;
  AviasalesAPI* aviasales_api_;
  ChatGPTAPI* chatgpt_api_;
};

}  // namespace my_service
