#pragma once

#include <string>
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
#include "chatgptAPI.hpp"
#include "userver/formats/json/value_builder.hpp"

namespace my_service {

class HttpClientComponent final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "http-client-component";

  HttpClientComponent(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
      http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()),
      aviasales_api_(&context.FindComponent<AviasalesAPI>()),
      chatgpt_api_(&context.FindComponent<ChatGPTAPI>()) {}
      
      
  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
  const auto& json = userver::formats::json::FromString(request.RequestBody());
  std::string from = json["origin"].As<std::string>();
  std::string to = json["destination"].As<std::string>();

  std::string response = aviasales_api_->GetIATACode(from, to);
  auto iata_json = userver::formats::json::FromString(response);

  std::vector<Attraction> attractions = chatgpt_api_->GetTravelInfo(to);

  userver::formats::json::ValueBuilder response_json;
  response_json["origin"] = iata_json["origin"];
  response_json["destination"] = iata_json["destination"];

  userver::formats::json::ValueBuilder attractions_array = response_json["attraction"];
  for (const auto& attraction : attractions) {
      userver::formats::json::ValueBuilder attraction_json;
      attraction_json["name"] = attraction.name;
      attraction_json["description"] = attraction.description;
      attractions_array.PushBack(std::move(attraction_json));
  } 
  return userver::formats::json::ToString(response_json.ExtractValue());
  }

 private:
 userver::clients::http::Client* http_client_;
  AviasalesAPI* aviasales_api_;
  ChatGPTAPI* chatgpt_api_;
};

}  // namespace my_service
