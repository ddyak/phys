
CREATE TABLE IF NOT EXISTS events
(
    event VARCHAR(30),
    time  TIMESTAMP
);

INSERT INTO events(event, time)
VALUES ('SMTH1', '2018-01-20 12:21:01'),
       ('SMTH2', '2018-10-01 15:07:37'),
       ('SMTH3', '2018-02-13 9:15:18'),
       ('SMTH4', '2018-11-07 11:43:21'),
       ('SMTH5', '2018-06-02 10:11:31'),
       ('SMTH6', '2018-08-19 14:09:20');

SELECT DISTINCT time::TIMESTAMP::date
FROM events
WHERE time::TIMESTAMP::TIME <= '12:00:00'
  AND time::TIMESTAMP::TIME >= '8:00:00';

SELECT * FROM events;