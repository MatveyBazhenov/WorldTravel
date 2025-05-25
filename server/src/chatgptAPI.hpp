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
#include "userver/storages/secdist/component.hpp"
#include "userver/yaml_config/merge_schemas.hpp"

struct Attraction {
    std::string name;
    std::string description;
};

class ChatGPTAPI final : public userver::components::LoggableComponentBase {
    public:
        static constexpr std::string_view kName = "yandexgpt-api";
    
        ChatGPTAPI(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
            : userver::components::LoggableComponentBase(config, context),
              http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()) {
            api_url_ = config["url"].As<std::string>();

            auto& secdist = context.FindComponent<userver::components::Secdist>().Get();
            secrets_ = secdist.Get<userver::formats::json::Value>();

            api_key = secrets_["yandex_key"].As<std::string>();
            folder_id_ = secrets_["folder_id"].As<std::string>();
        }

        static userver::yaml_config::Schema GetStaticConfigSchema() {
            return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
              type: object
              description: "YandexGPT API component"
              properties:
                url:
                  type: string
                  description: "YandexGPT API URL"
                iam-token:
                  type: string
                  description: "IAM-токен для доступа к Yandex Cloud"
                folder-id:
                  type: string
                  description: "Yandex Cloud Folder ID"
              additionalProperties: false
            )");
        }
    
        std::vector<Attraction> GetTravelInfo(userver::formats::json::Value json) {
            std::string city = json["origin"].As<std::string>();
            std::vector<Attraction> attractions;
    
            userver::formats::json::ValueBuilder request;
            request["modelUri"] = fmt::format("gpt://{}/yandexgpt/latest", folder_id_);
            request["completionOptions"]["stream"] = false;
            request["completionOptions"]["temperature"] = 0.7;
    
            userver::formats::json::ValueBuilder user_msg;
            user_msg["role"] = "user";
            user_msg["text"] = fmt::format(
                "Составь JSON-массив из 5 главных достопримечательностей города {}. Укажи \"name\" и \"description\" для каждой.", city);
    
            request["messages"].PushBack(std::move(user_msg));

    
            auto response = http_client_->CreateRequest()
            .post(api_url_)
            .timeout(std::chrono::seconds(10))
            .headers({
                {"Authorization", "Bearer " + api_key},
                {"Content-Type", "application/json"}
            })
            .data(userver::formats::json::ToString(request.ExtractValue()))
            .perform();
    
            const auto& json_response = userver::formats::json::FromString(response->body());
    
            if (!json_response.HasMember("result")) return attractions;

            const auto& result = json_response["result"];

            if (!result.HasMember("alternatives")) return attractions;
    
            const auto& alternatives = result["alternatives"];
            if (alternatives.IsEmpty()) return attractions;
    
            const auto& text = alternatives[0]["message"]["text"].As<std::string>();
    
            const auto& parsed_attractions = userver::formats::json::FromString(text);
            if (!parsed_attractions.IsArray()) return attractions;
    
            for (const auto& item : parsed_attractions) {
                Attraction attraction;
                if (item.HasMember("name")) {
                    attraction.name = item["name"].As<std::string>();
                }
                if (item.HasMember("description")) {
                    attraction.description = item["description"].As<std::string>();
                }
                attractions.push_back(attraction);
            }
    
            return attractions;
        }

private:
    userver::clients::http::Client* http_client_;
    std::string api_url_;
    std::string api_key;
    std::string folder_id_;
    userver::formats::json::Value secrets_;
};