
CREATE TABLE IF NOT EXISTS users (
  id bigserial PRIMARY KEY,
  username varchar NOT NULL,
  password varchar NOT NULL,
  email varchar NOT NULL,
  created_at timestamptz DEFAULT now(),
  updated_at timestamptz DEFAULT now(),
  UNIQUE (username, email)
);

CREATE INDEX ON users (username);
CREATE INDEX ON users (email);

INSERT INTO users (username, password, email) VALUES ('defaultuser', 'defaultuser123', 'defaultuser@gmail.com');