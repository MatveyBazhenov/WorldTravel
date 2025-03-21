import pytest
from testsuite.databases import pgsql
import uuid

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_successful_login(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': 'correct_password'}
    )
    
    assert response.status == 200
    assert response.text == "Welcome, existing_user\n"

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_user_not_found(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'not_existing_user', 'password': 'another_password'}
    )
    assert response.status == 404
    assert response.text == "Unknown user\n"

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_wrong_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': 'wrong_password'}
    )
    assert response.status == 401
    assert response.text == "Incorrect password\n"

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': '1' * 51}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_username(service_client):
    response = await service_client.post(
        '/login',
        json={'login': '1' * 51, 'password': 'correct_password'}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'existing_user', 'password': ''}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/login',
        json={'login': '', 'password': 'correct_password'}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_some_users(service_client):
    response = await service_client.post(
        '/login',
        json={'login': 'first_user', 'password': 'first_password'}
    )
    assert response.status == 200
    assert response.text == "Welcome, first_user\n"

    response = await service_client.post(
        '/login',
        json={'login': 'second_user', 'password': 'second_password'}
    )
    assert response.status == 200
    assert response.text == "Welcome, second_user\n"

    response = await service_client.post(
        '/login',
        json={'login': 'first_user', 'password': 'wrong_password'}
    )
    assert response.status == 401
    assert response.text == "Incorrect password\n"