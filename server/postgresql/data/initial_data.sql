INSERT INTO WorldTravel.users(username, password, count)
VALUES ('test_user', 'test_password', 1)
ON CONFLICT (username)
DO NOTHING;
