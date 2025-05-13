import pytest

@pytest.fixture
def mock_aviasales(mockserver):
    @mockserver.handler('/iata')
    async def _mock_iata(request):
        return mockserver.make_response(
            json={
                "origin" :{
                    "iata" : "MOW"
                },
                "destination" :{
                    "iata" : "KHV"
                }
            },
            status=200
        )

    @mockserver.handler('/prices')
    async def _mock_prices(request):
        return mockserver.make_response(
            json={
                "success": True,
                "data": [{
                    "origin": "MOW",
                    "destination": "KHV",
                    "departure_at": "2025-05-15T08:25:00+03:00",
                    "price": 25636
                }]
            },
            status=200
        )

@pytest.mark.asyncio
async def test_http_client_mock(mock_aviasales, service_client):
    response = await service_client.post(
        '/http-client',
        json={
            "origin": "Москва",
            "destination": "Хабаровск",
            "departure_at": "2025-05-15",
            "price": 40000,
        },
    )
    assert response.status == 200
    body = response.json()
    assert body == {
        "origin_city": "Москва",
        "destination_city": "Хабаровск",
        "origin_IATA": "MOW",
        "destination_IATA": "KHV",
        "departure_at": "2025-05-15T08:25:00+03:00",
        "price": 25636
    }
