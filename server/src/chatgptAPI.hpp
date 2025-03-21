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
          http_client_(&context.FindComponent<userver::components::HttpClient>().GetHttpClient()),
          pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-database").GetCluster()) {
        api_url_ = config["api"]["chatgpt_url"].As<std::string>();
        api_key_ = config["api"]["chatgpt_api_key"].As<std::string>();
    }

    std::vector<Attraction> GetTravelInfo(const std::string& from, const std::string& to) {
        auto trx = pg_cluster_->Begin("select_attractions");
        auto result = trx.Execute("SELECT name, description FROM WorldTravel.attractions WHERE city = $1", to);
        
        std::vector<Attraction> attractions;
        if (!result.IsEmpty()) {
            for (const auto& row : result) {
                attractions.push_back({row["name"].As<std::string>(), row["description"].As<std::string>()});
            }
            return attractions;
        }

        auto request_body = fmt::format(R"({{
            "model": "gpt-4",
            "messages": [
                {{"role": "system", "content": "Provide a structured JSON array with the main attractions of a city."}},
                {{"role": "user", "content": "List the top attractions in {} with short descriptions as JSON."}}
            ]
        }})", to);

        auto request = http_client_->CreateRequest()
            .post(api_url_)
            .timeout(std::chrono::seconds(5))
            .header("Authorization", fmt::format("Bearer {}", api_key_))
            .header("Content-Type", "application/json")
            .body(request_body)
            .perform();

        auto json_response = userver::formats::json::FromString(request->body());
        auto attractions_json = json_response["choices"][0]["message"]["content"];

        for (const auto& item : attractions_json.AsArray()) {
            Attraction attraction = {item["name"].As<std::string>(), item["description"].As<std::string>()};
            attractions.push_back(attraction);

            trx.Execute("INSERT INTO WorldTravel.attractions (city, name, description) VALUES ($1, $2, $3)",
                        to, attraction.name, attraction.description);
        }
        
        trx.Commit();
        return attractions;
    }

private:
    userver::clients::http::Client* http_client_;
    userver::storages::postgres::Cluster* pg_cluster_;
    std::string api_url_;
    std::string api_key_;
};
