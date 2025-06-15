#include "userver/formats/json/inline.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/server/http/http_status.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"

#include <fmt/format.h>

#include <exception>
#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

namespace my_service { 
class AccountHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-account";

  AccountHandler(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context), 
      postgres_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

      std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext&) const override {
      const auto& user_key = request.GetArg("user_key");

      if (user_key.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return R"({"error": "user_key parameter is required"})";
      }
    
      constexpr const char* kSelectTripQuery = R"(
      SELECT 
        trip_id,
        origin_city,
        destination_city,
        origin_IATA,
        destination_IATA,
        departure_at,
        price,
        description_city
      FROM WorldTravel.trips
      WHERE user_key = $1
      ORDER BY trip_id
    )";

      try {
        auto result = postgres_->Execute(userver::storages::postgres::ClusterHostType::kSlave, kSelectTripQuery, user_key);
        userver::formats::json::ValueBuilder json;
        json["trips"] = userver::formats::json::MakeArray();
        auto vector_trips = json["trips"];

        for (const auto& row : result) {
            userver::formats::json::ValueBuilder trip;
            trip["trip_id"] = row["trip_id"].As<int>();
            trip["origin_city"] = row["origin_city"].As<std::string>();
            trip["destination_city"] = row["destination_city"].As<std::string>();
            trip["origin_IATA"] = row["origin_IATA"].As<std::string>();
            trip["destination_IATA"] = row["destination_IATA"].As<std::string>();
            if (!row["departure_at"].IsNull()) {
              trip["departure_at"] = row["departure_at"].As<std::string>();
          } else {
              trip["departure_at"] = "";
          }
            trip["price"] = row["price"].As<int>();
            if (!row["description_city"].IsNull()) {
                try {
                    const auto& raw_str = row["description_city"].As<std::string>();
                    if (!raw_str.empty()) {
                      try {
                        trip["description_city"] = userver::formats::json::FromString(raw_str);
                      } catch (std:: exception&) {
                        trip["description_city"] = raw_str;
                      }  
                    } else {
                        trip["description_city"] = userver::formats::json::MakeObject();
                    }
                } catch (const std::exception& e) {
                    trip["description_city"] = userver::formats::json::MakeObject(); 
                }
            } else {
                trip["description_city"] = userver::formats::json::MakeObject(); 
            }
            vector_trips.PushBack(std::move(trip));
          }

          return userver::formats::json::ToString(json.ExtractValue());

      } catch(const std::exception& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kInternalServerError);
        return fmt::format(R"({{"error": "{}"}})", e.what());
      }
    }
private:
    userver::storages::postgres::ClusterPtr postgres_;
};

}  // namespace my_service