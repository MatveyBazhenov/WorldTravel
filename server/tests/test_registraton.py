import pytest
from testsuite.databases import pgsql

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_successful_registration(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'secure_password'}
    )
    assert response.status == 200
    assert response.json() == {"status": "ok", "user_key": "new_user"}

async def test_data_in_table(service_client, pgsql,):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': 'secure_password'}
    )
    cursor = pgsql['db_1'].cursor()
    cursor.execute('SELECT * FROM WorldTravel.users;')
    assert list(cursor) == [('new_user', 'secure_password', 1)]

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
    assert response.json() == {"status": "error", "message": "User new_user already exists"}

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': '111111111111111111111111111111111111111111111111111'}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_too_long_username(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': '111111111111111111111111111111111111111111111111111', 'password': 'secure_password'}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'new_user', 'password': ''}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_empty_password(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': '', 'password': 'secure_password'}
    )
    assert response.status == 400
    assert response.json() == {"status": "error", "message": "Invalid username or password. Must be non-empty and up to 50 characters"}

@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_some_users(service_client):
    response = await service_client.post(
        '/registration',
        json={'username': 'first_user', 'password': 'first_password'}
    )
    assert response.status == 200
    assert response.json() == {"status": "ok", "user_key": "first_user"}

    response = await service_client.post(
        '/registration',
        json={'username': 'second_user', 'password': 'second_password'}
    )
    assert response.status == 200
    assert response.json() == {"status": "ok", "user_key": "second_user"}

    response = await service_client.post(
        '/registration',
        json={'username': 'first_user', 'password': 'another_password'}
    )
    assert response.status == 409
    assert response.json() == {"status": "error", "message": "User first_user already exists"}

