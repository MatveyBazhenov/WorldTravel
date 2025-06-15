#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>


#include "login.hpp"
#include "registration.hpp"
#include "find.hpp"
#include "aviasalesAPI.hpp"
#include "save_trip.hpp"
#include "account.hpp"
#include "userver/storages/secdist/component.hpp"
#include "userver/storages/secdist/provider_component.hpp"
#include "chatgptAPI.hpp"

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<userver::components::Postgres>("postgres-db-1")
                            .Append<userver::clients::dns::Component>()
                            .Append<AviasalesAPI>()
                            .Append<ChatGPTAPI>()
                            .Append<userver::components::DefaultSecdistProvider>()
                            .Append<userver::components::Secdist>()
                            .Append<my_service::HttpClientComponent>()
                            .Append<my_service::SaveTripHandler>()
                            .Append<my_service::AccountHandler>();
                            

  my_service::AppendLogin(component_list);
  my_service::AppendRegistration(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
