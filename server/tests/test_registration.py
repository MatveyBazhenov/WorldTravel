import pytest
from testsuite.databases import pgsql
import uuid


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_successful_registration(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'secure_password'}
    )

    assert response.status == 200
    response_json = response.json()

    assert "status" in response_json
    assert response_json["status"] == "ok"

    assert "user_key" in response_json
    user_key = response_json["user_key"]
    uuid.UUID(user_key).version == 7


async def test_data_in_table(service_client, pgsql,):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'secure_password'}
    )
    assert response.status == 200
    response_json = response.json()
    cursor = pgsql['db_1'].cursor()
    cursor.execute('SELECT * FROM WorldTravel.users;')
    # assert list(cursor) == [('new_user', 'secure_password', 1)]
    row = cursor.fetchone()
    assert row[0] == 'new_user'
    assert row[1] == 'secure_password'
    assert row[2] == response_json["user_key"]
    assert row[3] == 1


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_user_already_exists(service_client):
    await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'secure_password'}
    )
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'another_password'}
    )
    assert response.status == 409
    assert response.json() == {"status": "error",
                               "message": "User new_user already exists"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user',
              'password': '111111111111111111111111111111111111111111111111111'}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_username(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': '111111111111111111111111111111111111111111111111111',
              'password': 'secure_password'}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': ''}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': '', 'password': 'secure_password'}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}
@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_some_users(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'first_user', 'password': 'first_password'}
    )
    assert response.status == 200
    response_json = response.json()

    assert "status" in response_json
    assert response_json["status"] == "ok"

    assert "user_key" in response_json
    user_key = response_json["user_key"]
    uuid.UUID(user_key).version == 7

    response = await service_client.post(
        '/registration',
        json={'username': 'second_user', 'password': 'second_password'}
    )
    assert response.status == 200
    response_json = response.json()

    assert "status" in response_json
    assert response_json["status"] == "ok"

    assert "user_key" in response_json
    user_key = response_json["user_key"]
    uuid.UUID(user_key).version == 7

    response = await service_client.post(
        '/registration',
        json={'username': 'first_user', 'password': 'another_password'}
    )
    assert response.status == 409
    assert response.json() == {"status": "error",
                               "message": "User first_user already exists"}

import asyncio
import logging
import time

async def test_batch_registration(service_client):
    async def _register_user(user_num: int) -> bool:

        username = f'user_{user_num}_{uuid.uuid4().hex[:8]}'
        password = f'password_{user_num}'

        request = await service_client.post(
            '/registration',
            json={'username': username, 'password': password}
        )
        return request.status == 200
    
    start_time = time.time()

    results = await asyncio.gather(
        *(_register_user(i) for i in range(10000))
    )

    elapsed_time = time.time() - start_time
    success = sum(results)

    # assert False, f'Total: {elapsed_time} seconds\n Successful registrations: {success}\n Requests per second: {10000 / elapsed_time}'
    # message after assert False -- Total: 13.196520328521729 seconds Successful registrations: 10000 Requests per second: 757.7755158976971

    assert all(results), f"Not all registrations were successful. only {success} out of 10000"
    #logging.info(f'Elapsed time: {time.time() - start_time}')
