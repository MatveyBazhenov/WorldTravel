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
