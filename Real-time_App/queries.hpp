#ifndef QUERIES_H
#define QUERIES_H

#define SELECT_ALL_DATA "SELECT attempt_id, check_attempt.card, UNIX_TIMESTAMP(time), device_name, status_code, card.associated_name, card.associated_phone_number, card.granted_access_level FROM check_attempt LEFT JOIN card ON check_attempt.card = card.card_uid"
#define SELECT_USER "SELECT * FROM card WHERE card_code=\'" + card_code + "\'"
#define INSERT_ATTEMPT "INSERT INTO check_attempt (card, time, device_name, status_code) VALUES ( " + card_uid + ", CURRENT_TIMESTAMP, \'" + device_name + "\', " + std::to_string(status_code) + " )"

#endif // queries
