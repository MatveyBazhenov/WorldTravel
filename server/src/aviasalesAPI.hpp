#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <chrono>
#include <string>
#include <userver/components/loggable_component_base.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include "userver/components/component_base.hpp"
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/yaml_config/merge_schemas.hpp"
#include <userver/clients/http/component.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/yaml_config/yaml_config.hpp>
#include <userver/formats/json/serialize.hpp>

class AviasalesAPI final : public userver::components::LoggableComponentBase{
    public:
    [[maybe_unused]] static constexpr std::string_view kName = "aviasales-api";

    AviasalesAPI(const userver::components::ComponentConfig& config, 
        const userver::components::ComponentContext& context)
        : userver::components::LoggableComponentBase(config, context),
        http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()){
            url_IATA_ = config["url-IATA"].As<std::string>();
        }

        static userver::yaml_config::Schema GetStaticConfigSchema() {
            return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
                type: object
                description: "URL for IATA code"
                additionalProperties: false
                properties:
                    url-IATA:
                        type: string
                        description: "Base URL for the Aviasales API"
            )");
        }

    std::string GetIATACode(std::string from, std::string to){
        std::string url = url_IATA_ + fmt::format("из%20{}%20в%20", from) + to;
        auto response = http_client_->CreateRequest()
        .get(url)
        .timeout(std::chrono::seconds(5))
        .perform();
        auto json = userver::formats::json::FromString(response->body());

        std::string origin = json["origin"]["iata"].As<std::string>();
        std::string destination = json["destination"]["iata"].As<std::string>();
        std::string ans = fmt::format(R"({{"origin": "{}", "destination": "{}"}})", origin, destination);
        return ans;
    }

    private:
        userver::clients::http::Client* http_client_;
        std::string url_IATA_;
};