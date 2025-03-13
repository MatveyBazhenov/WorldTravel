INSERT INTO WorldTravel.users(username, password, user_key,count)
VALUES ('test_user', 'test_password', 'key_123', 1)
ON CONFLICT (username)
DO NOTHING;
