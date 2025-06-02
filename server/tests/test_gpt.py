import pytest 

@pytest.mark.asyncio
async def test_mock_gpt_response(mock_gpt, service_client):
    response = await service_client.get(
        '/gpt', 
        json={"origin":"Хабаровск"}
    )
    assert response.status == 200
    data = response.json()

    assert isinstance(data, list)
    assert len(data) == 5
    