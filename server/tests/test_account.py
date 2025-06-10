import pytest
from testsuite.databases import pgsql
import json

@pytest.mark.pgsql('db_1')
async def test_get_trips_success(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('test_user', 'pass', 'test_key')
    )
    cursor.execute(
        """
        INSERT INTO WorldTravel.trips 
        (user_key, origin_city, destination_city, origin_IATA, destination_IATA, 
         departure_at, price, description_city) 
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
        """,
        (
            'test_key', 'Moscow', 'London', 'SVO', 'LHR', 
            '2025-10-10T17:20:00+03:00', 25000,
            json.dumps([{"name": "Big Ben", "description": "Clock tower"}])
        )
    )

    response = await service_client.get(
        '/account',
        params={'user_key': 'test_key'}
    )
    
    assert response.status == 200
    body = response.json()
    assert 'trips' in body
    assert len(body['trips']) == 1
    assert body['trips'][0]['origin_city'] == 'Moscow'
    assert body['trips'][0]['description_city'][0]['name'] == 'Big Ben'
    assert body['trips'][0]['description_city'][0]['description'] == 'Clock tower'


async def test_get_trips_empty(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('empty_user', 'pass', 'empty_key')
    )

    response = await service_client.get(
        '/account',
        params={'user_key': 'empty_key'}
    )
    
    assert response.status == 200
    assert response.json() == {'trips': []}


async def test_get_trips_missing_user_key(service_client):
    response = await service_client.get('/account')
    
    assert response.status == 400
    assert response.json() == {'error': 'user_key parameter is required'}


async def test_get_multiple_trips(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('multi_user', 'pass', 'multi_key')
    )
    
    for i in range(1, 3):
        cursor.execute(
            """
            INSERT INTO WorldTravel.trips 
            (user_key, origin_city, destination_city, origin_IATA, destination_IATA, 
             departure_at, price, description_city) 
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
            """,
            (
                'multi_key', f'City{i}', f'Dest{i}', f'ORI{i}', f'DES{i}',
                f'2025-01-{i:02d}T12:00:00+03:00', 10000 + i,
                json.dumps([
                    {
                        "name": f"Attraction {i}",
                        "description": f"Detailed description for attraction {i}",
                    },
                    {
                        "name": f"Park {i}",
                        "description": f"Beautiful green park #{i}",
                    }
                ])
            )
        )


    response = await service_client.get(
        '/account',
        params={'user_key': 'multi_key'}
    )
    
    assert response.status == 200
    body = response.json()
    assert len(body['trips']) == 2
    assert body['trips'][0]['origin_city'] == 'City1'
    assert body['trips'][1]['origin_city'] == 'City2'
    assert body['trips'][0]['description_city'][0]['name'] == 'Attraction 1'
    assert body['trips'][0]['description_city'][1]['name'] == 'Park 1'
    assert body['trips'][1]['description_city'][0]['description'] == 'Detailed description for attraction 2'