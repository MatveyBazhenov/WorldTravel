DROP SCHEMA IF EXISTS WorldTravel CASCADE;

CREATE SCHEMA IF NOT EXISTS WorldTravel;

CREATE TABLE IF NOT EXISTS WorldTravel.users (
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    user_key TEXT PRIMARY KEY,
    count INTEGER DEFAULT(1)
);

CREATE TABLE IF NOT EXISTS  WorldTravel.trips(
    trip_id SERIAL PRIMARY KEY,
    user_key TEXT REFERENCES WorldTravel.users(user_key) ON DELETE CASCADE,
    origin_city TEXT NOT NULL,
    destination_city TEXT NOT NULL,
    origin_IATA TEXT NOT NULL,
    destination_IATA TEXT NOT NULL,
    departure_at TEXT NOT NULL,
    price INTEGER NOT NULL
);
