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

struct Attraction {
    std::string name;
    std::string description;
};

class ChatGPTAPI final : public userver::components::LoggableComponentBase {
public:
    [[maybe_unused]] static constexpr std::string_view kName = "chatgpt-api";

    ChatGPTAPI(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context)
: userver::components::LoggableComponentBase(config, context),
 http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()) {
            api_url_ = config["url-chatgpt"].As<std::string>();
            api_key_ = config["key-chatgpt"].As<std::string>();
        }

    static userver::yaml_config::Schema GetStaticConfigSchema() {
            return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
              type: object
              description: "ChatGPT API component"
              properties:
                url-chatgpt:
                  type: string
                  description: "URL for the ChatGPT API"
                key-chatgpt:
                  type: string
                  description: "API Key for ChatGPT"
              additionalProperties: false
            )");
    }

    std::vector<Attraction> GetTravelInfo(const std::string& to) {
        std::vector<Attraction> attractions;

        userver::formats::json::ValueBuilder request_body;
        userver::formats::json::ValueBuilder messages;

        userver::formats::json::ValueBuilder system_message;
        system_message["role"] = "system";
        system_message["content"] = "Provide a structured JSON array with the five main attractions of a city.";
        messages.PushBack(std::move(system_message));

        userver::formats::json::ValueBuilder user_message;
        user_message["role"] = "user";
        user_message["content"] = fmt::format("List the five top attractions in {} with short descriptions as JSON.", to);
        messages.PushBack(std::move(user_message));

        request_body["model"] = "gpt-3.5-turbo";
        request_body["messages"] = std::move(messages);

        auto response = http_client_->CreateRequest()
        .post(api_url_)
        .timeout(std::chrono::seconds(10))
        .headers({{"Authorization", "Bearer " + api_key_}, {"Content-Type", "application/json"}})
        .perform();


        const auto& json_response = userver::formats::json::FromString(response->body());
        if (json_response.HasMember("choices") && json_response["choices"].IsArray()) {
            auto choices = json_response["choices"];
            if (!choices.IsEmpty()) {
                auto content = choices[0]["message"]["content"].As<std::string>();
                const auto& attractions_json = userver::formats::json::FromString(content);

                if (attractions_json.IsArray()) {
                    for (auto it = attractions_json.begin(); it != attractions_json.end(); ++it) { 
                        const auto& item = *it;
                        Attraction attraction;
                        if (item.IsObject() && item.HasMember("name") && item["name"].IsString()) {
                            attraction.name = item["name"].As<std::string>();
                        }
                        if (item.IsObject() && item.HasMember("description") && item["description"].IsString()) {
                            attraction.description = item["description"].As<std::string>();
                        }
                        attractions.push_back(attraction);
                    }
                }
            }
        }
        return attractions;
    }

private:
    userver::clients::http::Client* http_client_;
    std::string api_url_;
    std::string api_key_;
};