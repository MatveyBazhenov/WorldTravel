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
            json.dumps({"Big Ben": "Clock tower"})
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
    assert body['trips'][0]['description_city'] == {"Big Ben": "Clock tower"}


@pytest.mark.pgsql('db_1')
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


@pytest.mark.pgsql('db_1')
async def test_get_trips_missing_user_key(service_client):
    response = await service_client.get('/account')

    assert response.status == 400
    assert response.json() == {'error': 'user_key parameter is required'}


@pytest.mark.pgsql('db_1')
async def test_get_multiple_trips(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('multi_user', 'pass', 'multi_key')
    )

    trips_data = [
        {
            'origin': 'City1',
            'dest': 'Dest1',
            'origin_iata': 'ORI1',
            'dest_iata': 'DES1',
            'date': '2025-01-01T12:00:00+03:00',
            'price': 10001,
            'description': {"Attraction 1": "Detailed description 1", "Park 1": "Beautiful park 1"}
        },
        {
            'origin': 'City2',
            'dest': 'Dest2',
            'origin_iata': 'ORI2',
            'dest_iata': 'DES2',
            'date': '2025-01-02T12:00:00+03:00',
            'price': 10002,
            'description': {"Attraction 2": "Detailed description 2", "Park 2": "Beautiful park 2"}
        }
    ]

    for trip in trips_data:
        cursor.execute(
            """
            INSERT INTO WorldTravel.trips 
            (user_key, origin_city, destination_city, origin_IATA, destination_IATA, 
             departure_at, price, description_city) 
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
            """,
            (
                'multi_key', trip['origin'], trip['dest'],
                trip['origin_iata'], trip['dest_iata'],
                trip['date'], trip['price'],
                json.dumps(trip['description'])
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
    assert body['trips'][0]['description_city'] == {
        "Attraction 1": "Detailed description 1",
        "Park 1": "Beautiful park 1"
    }

    assert body['trips'][1]['origin_city'] == 'City2'
    assert body['trips'][1]['description_city'] == {
        "Attraction 2": "Detailed description 2",
        "Park 2": "Beautiful park 2"
    }

import asyncio
import time
import uuid

@pytest.mark.pgsql('db_1')
async def test_batch_account(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    user_keys = [str(uuid.uuid4()) for _ in range(10000)]
    cursor.executemany(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        [(f"user_{i}", f"pass_{i}", user_keys[i]) for i in range(10000)]
    )
    trips = []
    for i, user_key in enumerate(user_keys):
        for j in range(3): 
            trip = {
                'user_key': user_key,
                'origin': f'City_{i}_{j}',
                'dest': f'Dest_{i}_{j}',
                'origin_iata': f'ORI{i}{j}',
                'dest_iata': f'DES{i}{j}',
                'date': f'2025-01-01T12:00:00+03:00',
                'price': 10000,
                'description': {
                    f"Attraction_{i}_{j}": f"Description_{i}_{j}",
                    f"Park_{i}_{j}": f"Beautiful park {i}_{j}"
                }
            }
            trips.append((
                user_key,
                trip['origin'],
                trip['dest'],
                trip['origin_iata'],
                trip['dest_iata'],
                trip['date'],
                trip['price'],
                json.dumps(trip['description'])
            ))

    cursor.executemany(
        """
        INSERT INTO WorldTravel.trips 
        (user_key, origin_city, destination_city, origin_IATA, destination_IATA, 
         departure_at, price, description_city) 
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
        """,
        trips
    )

    async def _account_user(user_num: int):
        response = await service_client.get(
            '/account',
            params={'user_key': user_keys[user_num]}
        )
        return response.status == 200

    start_time = time.time()

    results = await asyncio.gather(
        *(_account_user(i) for i in range(10000))
    )
    
    elapsed_time = time.time() - start_time
    success = sum(results)
    
    # assert False, f'Total: {elapsed_time} seconds\n  Successful logins: {success}\n Logins per second: {10000 / elapsed_time}'
    # Total: 14.554787635803223 seconds Successful logins: 10000 Logins per second: 687.059148523821
    
    assert all(results), f"Not all requests were successful. Success: {success} out of 10000"