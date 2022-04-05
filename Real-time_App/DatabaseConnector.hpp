#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <mysqlx/xdevapi.h>

#include "IOManager.hpp"

class DatabaseConnector
{
private:
    mysqlx::Session m_session;
    bool m_is_connected;
    IOManager &m_IO_manager;

    mysqlx::SqlResult executeQuery(const std::string &query);
    int validateEmployee(const std::string &card_code, const int access_level, std::string &card_uid);
    int writeAttempt(const std::string &card_uid, const std::string &device_name, const int status_code);

    std::string hexToString(const uint8_t *data, const size_t size) const;

    int log(const std::string &msg) const;

public:
    DatabaseConnector(const std::string &db_info, IOManager &IO_manager);
    ~DatabaseConnector();

    int checkAttempt(const std::string &card_code_hex, const std::string &device_name, const int access_level);
};

#endif // DatabaseConnector
