import pytest
from testsuite.databases import pgsql

@pytest.mark.pgsql('db_1')
async def test_successful_save_trip(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('existing_user', 'correct_password', '1111aaaa2222bbbb3333cccc4444')
    )
    response = await service_client.post(
        '/save-trip',
        json={
            "user_key" : "1111aaaa2222bbbb3333cccc4444",
            "origin_city":"Start-city",
            "destination_city":"Finish-city",
            "origin_IATA":"STR",
            "destination_IATA":"FIN",
            "departure_at":"2025-10-10T17:20:00+03:00",
            "price":25000
        },
    )
    
    assert response.status == 200
    body = response.json()
    assert body == {"status": "ok"}


async def test_trip_data_in_table(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('existing_user', 'correct_password', '1111aaaa2222bbbb3333cccc4444')
    )
    response = await service_client.post(
        '/save-trip',
        json={
            "user_key" : "1111aaaa2222bbbb3333cccc4444",
            "origin_city":"Start-city",
            "destination_city":"Finish-city",
            "origin_IATA":"STR",
            "destination_IATA":"FIN",
            "departure_at":"2025-10-10T17:20:00+03:00",
            "price":25000
        },
    )

    assert response.status == 200
    cursor.execute('SELECT * FROM WorldTravel.trips;')
    row = cursor.fetchone()
    assert row[0] == 1
    assert row[1] == '1111aaaa2222bbbb3333cccc4444'
    assert row[2] == 'Start-city'
    assert row[3] == 'Finish-city'
    assert row[4] == 'STR'
    assert row[5] == 'FIN'
    assert row[6] == '2025-10-10T17:20:00+03:00' 
    assert row[7] == 25000 


async def test_two_trips(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('existing_user', 'correct_password', '1111aaaa2222bbbb3333cccc4444')
    )

    response = await service_client.post(
        '/save-trip',
        json={
            "user_key" : "1111aaaa2222bbbb3333cccc4444",
            "origin_city":"First-start-city",
            "destination_city":"First-finish-city",
            "origin_IATA":"STR1",
            "destination_IATA":"FIN1",
            "departure_at":"2025-10-10T17:20:00+03:00",
            "price":11111
        },
    )
    assert response.status == 200
    response = await service_client.post(
        '/save-trip',
        json={
            "user_key" : "1111aaaa2222bbbb3333cccc4444",
            "origin_city":"Second-start-city",
            "destination_city":"Second-finish-city",
            "origin_IATA":"STR2",
            "destination_IATA":"FIN2",
            "departure_at":"2025-20-20T17:20:00+03:00",
            "price":22222
        },
    )
    assert response.status == 200


    cursor.execute('SELECT * FROM WorldTravel.trips;')
    rows = cursor.fetchall()
    assert rows[0][0] == 1
    assert rows[0][1] == '1111aaaa2222bbbb3333cccc4444'
    assert rows[0][2] == 'First-start-city'
    assert rows[0][3] == 'First-finish-city'
    assert rows[0][4] == 'STR1'
    assert rows[0][5] == 'FIN1'
    assert rows[0][6] == '2025-10-10T17:20:00+03:00' 
    assert rows[0][7] == 11111 

    assert rows[1][0] == 2
    assert rows[1][1] == '1111aaaa2222bbbb3333cccc4444'
    assert rows[1][2] == 'Second-start-city'
    assert rows[1][3] == 'Second-finish-city'
    assert rows[1][4] == 'STR2'
    assert rows[1][5] == 'FIN2'
    assert rows[1][6] == '2025-20-20T17:20:00+03:00' 
    assert rows[1][7] == 22222