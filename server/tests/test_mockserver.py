import pytest

@pytest.mark.asyncio
async def test_http_client_mock(mockserver, service_client):
    response = await service_client.get(
        '/http-client',
        json={
            "origin": "Москва",
            "destination": "Хабаровск",
            "departure_at": "2025-07-15",
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
        "departure_at": "2025-07-15T17:20:00+03:00",
        "price": 17000,
        "description":{
            "Площадь Ленина":"Центральная площадь Хабаровска.",
            "Утёс":"Смотровая площадка на Амур.",
            "Краевой музей им. Гродекова":"Исторический музей региона.",
            "Парк Северный":"Зелёная зона отдыха.",
            "Набережная Амура":"Живописное место для прогулок."
        }
    }
