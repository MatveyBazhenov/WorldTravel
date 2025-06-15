import pytest
from testsuite.databases import pgsql
import uuid


@pytest.mark.pgsql('db_1')
async def test_successful_login(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('existing_user', 'correct_password', '1111aaaa2222bbbb3333cccc4444')
    )

    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': 'correct_password'}
    )

    assert response.status == 200
    body = response.json()
    assert "status" in body
    assert body["status"] == "ok"
    assert "user_key" in body
    assert body["user_key"] == "1111aaaa2222bbbb3333cccc4444"


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_user_not_found(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'not_existing_user', 'password': 'another_password'}
    )
    assert response.status == 404
    assert response.json() == {
        "status": "error",
        "message": "Unknown user."
    }


@pytest.mark.pgsql('db_1')
async def test_wrong_password(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('existing_user', 'correct_password', '1111aaaa2222bbbb3333cccc4444')
    )

    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': 'wrong_password'}
    )
    assert response.status == 401
    assert response.json() == {
        "status": "error",
        "message": "Incorrect password."
    }


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': '1' * 51}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error",
        "message": "Invalid username or password. Must be non-empty and up to 50 characters"
    }


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_username(service_client):
    response = await service_client.post(
        '/login',
        json={'login': '1' * 51, 'password': 'correct_password'}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error",
        "message": "Invalid username or password. Must be non-empty and up to 50 characters"
    }


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': ''}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error",
        "message": "Invalid username or password. Must be non-empty and up to 50 characters"
    }


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_username(service_client):
    response = await service_client.post(
        '/login',
        json={'login': '', 'password': 'correct_password'}
    )
    assert response.status == 400
    assert response.json() == {
        "status": "error",
        "message": "Invalid username or password. Must be non-empty and up to 50 characters"
    }


@pytest.mark.pgsql('db_1')
async def test_some_users(service_client, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('first_user', 'first_password', '1111aaaa2222bbbb3333cccc4444')
    )
    cursor.execute(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        ('second_user', 'second_password', '2222bbbb3333cccc4444dddd5555')
    )

    response = await service_client.post(
        '/login',
        json={'login': 'first_user', 'password': 'first_password'}
    )
    assert response.status == 200
    assert (response.json())["status"] == "ok"

    response = await service_client.post(
        '/login',
        json={'login': 'second_user', 'password': 'second_password'}
    )
    assert response.status == 200
    assert (response.json())["status"] == "ok"

    response = await service_client.post(
        '/login',
        json={'login': 'first_user', 'password': 'wrong_password'}
    )
    assert response.status == 401
    assert response.json() == {
        "status": "error",
        "message": "Incorrect password."
    }

import asyncio
import time
import uuid

@pytest.mark.pgsql('db_1')
async def test_batch_login(service_client, pgsql):

    cursor = pgsql['db_1'].cursor()
    users = []
    for i in range(10000):
        username = f'user_{i}_{uuid.uuid4().hex[:8]}'
        password = f'password_{i}'
        user_key = str(uuid.uuid4())
        users.append((username, password, user_key))
    
    cursor.executemany(
        "INSERT INTO WorldTravel.users (username, password, user_key) VALUES (%s, %s, %s)",
        users
    )

    async def _login_user(user_num: int) -> bool:
        username, password, _ = users[user_num]
        response = await service_client.post(
            '/login',
            json={'login': username, 'password': password}
        )
        return response.status == 200

    start_time = time.time()

    results = await asyncio.gather(
        *(_login_user(i) for i in range(10000))
    )
    
    elapsed_time = time.time() - start_time
    success = sum(results)
    
    # assert False, f'Total: {elapsed_time} seconds\n  Successful logins: {success}\n Logins per second: {10000 / elapsed_time}'
    # message after assert False -- Total: 9.956981658935547 seconds Successful logins: 10000 Logins per second: 1004.3204198358493
    
    assert all(results), f"Not all logins were successful, only {success} out of 10000"