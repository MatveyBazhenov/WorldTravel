#include "userver/formats/json/serialize.hpp"
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

namespace my_service { 
class SaveTripHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-save_trip";

  SaveTripHandler(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context), 
      postgres_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

      std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext&) const override {
      const auto& json = userver::formats::json::FromString(request.RequestBody());
    
      // для ручных тестов
      // constexpr const char* kInsertUserQuery = R"(
      // INSERT INTO WorldTravel.users (username, password, user_key) VALUES ($1, $2, $3)
      // )";
      // postgres_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
      //                  kInsertUserQuery, "test-user", "test-password", "test");

      constexpr const char* kInsertTripQuery = R"(
      INSERT INTO WorldTravel.trips (user_key, origin_city, destination_city, origin_IATA, destination_IATA, departure_at, price, description_city) VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
      )";

      auto result = postgres_->Execute(
          userver::storages::postgres::ClusterHostType::kMaster,
          kInsertTripQuery,
          json["user_key"].As<std::string>(),
          json["origin_city"].As<std::string>(),
          json["destination_city"].As<std::string>(),
          json["origin_IATA"].As<std::string>(),
          json["destination_IATA"].As<std::string>(),
          json["departure_at"].As<std::string>(),
          json["price"].As<int>(),
          json["description_city"].As<std::string>()
      );

      return R"({"status": "ok"})";
    }

userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace my_service