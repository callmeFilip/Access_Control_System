#include "DatabaseConnector.hpp"
#include "queries.hpp"

#include <mysqlx/xdevapi.h>

#include <sstream>
#include <iomanip>

const int CODE_ACCESS_DENIED = 0;
const int CODE_ACCESS_GRANTED = 1;
const int CODE_ACCESS_DENIED_UNKNOWN = 2;

/**
 * @brief Construct a new Database Connector:: Database Connector object
 *
 * @param db_info "mysqlx://username:password@host:port/database_scheme"
 * @param IO_manager IO_manager instance responsible for logging
 */
DatabaseConnector::DatabaseConnector(const std::string &db_info, IOManager &IO_manager)
    : m_session(db_info), m_IO_manager(IO_manager)
{
#ifdef DEBUG
    log("Database Connection initialized");
#endif
}

/**
 * @brief Destroy the Database Connector:: Database Connector object
 * Close the connection
 */
DatabaseConnector::~DatabaseConnector()
{
    m_session.close();
#ifdef DEBUG
    log("Database Connection uninitialized");
#endif
}

/**
 * @brief Validate employee in database
 *
 * @param card_code Code of employee's card
 * @param access_level Device's access level
 * @param card_uid card_uid string variable. Used simply to return user's ID
 * @return int
 */
int DatabaseConnector::validateEmployee(const std::string &card_code, const int access_level, std::string &card_uid)
{
    mysqlx::SqlResult res = executeQuery(SELECT_USER);

    mysqlx::Row row = res.fetchOne();

    if (row.isNull())
    {
        return CODE_ACCESS_DENIED_UNKNOWN;
    }

    card_uid = std::to_string((int)row[0]);

    if (access_level > (int)row[3])
    {
        return CODE_ACCESS_DENIED;
    }
    return CODE_ACCESS_GRANTED;
}

/**
 * @brief Write the query to database
 *
 * @param card_uid Card uid
 * @param device_name Device name
 * @param status_code Status code
 * @return int
 */
int DatabaseConnector::writeAttempt(const std::string &card_uid, const std::string &device_name, const int status_code)
{
    executeQuery(INSERT_ATTEMPT);

    return 0;
}

/**
 * @brief Generate status of employee validation
 *
 * @param card_code_hex Hex of card code
 * @param device_name Device name
 * @param access_level Device access level
 * @return int
 */
int DatabaseConnector::checkAttempt(const std::string &card_code, const std::string &device_name, const int access_level)
{
    std::string card_uid = "NULL";
    int status_code = validateEmployee(card_code, access_level, card_uid);

    writeAttempt(card_uid, device_name, status_code);

    return status_code;
}

/**
 * @brief Execute the query
 *
 * @param query String query
 * @return mysqlx::SqlResult
 */
mysqlx::SqlResult DatabaseConnector::executeQuery(const std::string &query)
{
    return m_session.sql(query).execute();
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int DatabaseConnector::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}
