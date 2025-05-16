import pathlib

import pytest

from testsuite.databases.pgsql import discover
from testsuite import utils


pytest_plugins = ['pytest_userver.plugins.postgresql',
                 'pytest_userver.plugins.core']

USERVER_CONFIG_HOOKS = ['userver_config_Aviasales']

@pytest.fixture(scope='session')
def userver_config_Aviasales(mockserver_info):
    def do_patch(config_yaml, config_vars):
        components = config_yaml['components_manager']['components']
        components['aviasales-api']['url-IATA'] = mockserver_info.url('iata')
        components['aviasales-api']['url-aviasales'] = mockserver_info.url('prices')
    
    return do_patch


@pytest.fixture(autouse=True)
def mock_aviasales(mockserver):
    @mockserver.json_handler('/iata')
    def mock_iata(request):
        return {
            "origin" :{"iata" : "MOW"},
            "destination":{"iata" : "KHV"}
            }
    
    @mockserver.json_handler('/prices')
    def mock_prices(request):
        return {
            "data": [{
                    "flight_number":"5806",
                    "link":"/search/MOW1507KHV1?t=SU175260000017526525000004\
                    55SVOKHV_24c4838cc5a482258fb839f6db95cf22_16530&search_d\
                    ate=16052025&expected_price_uuid=cbff9c76-afaf-48e9-94d4\
                    -1c503908fd00&expected_price_source=share&expected_price\
                    _currency=rub&expected_price=16654",
                    "origin_airport":"SVO",
                    "destination_airport":"KHV",
                    "departure_at":"2025-07-15T17:20:00+03:00",
                    "airline":"SU",
                    "destination":"KHV",
                    "origin":"MOW",
                    "price":17000,
                    "return_transfers":0,
                    "duration":455,
                    "duration_to":455,
                    "duration_back":0,
                    "transfers":0
                }],
                "currency":"rub",
                "success": True
            }
    return mock_iata, mock_prices

@pytest.fixture(scope='session')
def service_source_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / 'postgresql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'service_template',  # service name that goes to the DB connection
        [service_source_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))
