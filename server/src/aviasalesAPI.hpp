#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <chrono>
#include <string>
#include <userver/clients/http/component.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/yaml_config/yaml_config.hpp>
#include "userver/components/component_base.hpp"
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/yaml_config/merge_schemas.hpp"

class AviasalesAPI final : public userver::components::LoggableComponentBase {
 public:
  [[maybe_unused]] static constexpr std::string_view kName = "aviasales-api";

  AviasalesAPI(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& context)
      : userver::components::LoggableComponentBase(config, context),
        http_client_(&context.FindComponent<userver::components::HttpClient>()
                          .GetHttpClient()) {
    url_IATA_ = config["url-IATA"].As<std::string>();
    url_aviasales_ = config["url-aviasales"].As<std::string>();
  }

  static userver::yaml_config::Schema GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<
        userver::components::LoggableComponentBase>(R"(
                type: object
                description: "URLs for IATA code and Aviasales API"
                additionalProperties: false
                properties:
                    url-IATA:
                        type: string
                        description: "Base URL for IATA code lookup"
                    url-aviasales:
                        type: string
                        description: "Base URL for the Aviasales API"
            )");
  }

  std::string GetIATACode(std::string from, std::string to) {
    std::string url = url_IATA_ + fmt::format("из%20{}%20в%20", from) + to;
    auto response = http_client_->CreateRequest()
                        .get(url)
                        .timeout(std::chrono::seconds(5))
                        .perform();
    const auto& json = userver::formats::json::FromString(response->body());

    userver::formats::json::ValueBuilder IATA_codes;
    IATA_codes["origin"] = json["origin"]["iata"];
    IATA_codes["destination"] = json["destination"]["iata"];
    return userver::formats::json::ToString(IATA_codes.ExtractValue());
  }

  std::string GetTickets(userver::formats::json::Value json) {
    std::string cities_IATA_codes =
        this->GetIATACode(json["origin"].As<std::string>(),
                          json["destination"].As<std::string>());
    const auto& cities_IATA_json =
        userver::formats::json::FromString(cities_IATA_codes);
    std::string origin = cities_IATA_json["origin"].As<std::string>();
    std::string destination = cities_IATA_json["destination"].As<std::string>();
    std::string departure_at = json["departure_at"].As<std::string>();
    std::string url =
        url_aviasales_ +
        fmt::format(
            R"(origin={}&destination={}&departure_at={}&unique=false&sorting=price&direct=false&currency=rub&limit=5&page=1&one_way=true&token={})",
            origin, destination, departure_at, api_token_);
    auto response = http_client_->CreateRequest()
                        .get(url)
                        .timeout(std::chrono::seconds(5))
                        .perform();
    // TODO int price = json["price"].As<int>();
    return response->body();
  }

 private:
  userver::clients::http::Client* http_client_;
  std::string url_IATA_;
  std::string url_aviasales_;
  const std::string api_token_ = "secret-token";
};