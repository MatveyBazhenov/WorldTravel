#pragma once

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

class AviasalesAPI final : public userver::components::LoggableComponentBase{
    public:
    [[maybe_unused]] static constexpr std::string_view kName = "aviasales-api";

    AviasalesAPI(const userver::components::ComponentConfig& config, 
        const userver::components::ComponentContext& context)
        : userver::components::LoggableComponentBase(config, context),
        http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()){
            url_ = config["url"].As<std::string>();
        }

        static userver::yaml_config::Schema GetStaticConfigSchema() {
            return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
                type: object
                description: "Configuration for AviasalesAPI component"
                additionalProperties: false
                properties:
                    url:
                        type: string
                        description: "Base URL for the Aviasales API"
            )");
        }
        
    

    std::string GetMyIP(){
        auto response = http_client_->CreateRequest()
        .get(url_)
        .perform();
        return response->body();
        // return url_;
    }

    private:
        userver::clients::http::Client* http_client_;
        std::string url_;
};