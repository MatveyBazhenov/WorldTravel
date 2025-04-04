DROP SCHEMA IF EXISTS WorldTravel CASCADE;

CREATE SCHEMA IF NOT EXISTS WorldTravel;

CREATE TABLE IF NOT EXISTS WorldTravel.users (
    username TEXT PRIMARY KEY,
    password TEXT NOT NULL,
    user_key TEXT NOT NULL,
    count INTEGER DEFAULT(1)
);