#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "ConnectionHandler.hpp"
#include "ServerSocket.hpp"

const int RECIEVE_LENGTH = 64;

const int CODE_ACCESS_DENIED = 0;
const int CODE_ACCESS_GRANTED = 1;
const int CODE_ACCESS_DENIED_UNKNOWN = 2;

const std::string delimiter = "&";

/**
 * @brief Construct a new Connection Handler:: Connection Handler object
 *
 * @param parent ServerSocket instantion that created this thread
 * @param client Client address
 * @param clien_socket Client socket
 * @param IO_manager IO_manager instance responsible for logging
 */
ConnectionHandler::ConnectionHandler(ServerSocket *parent, sockaddr_in *client, int client_socket, DatabaseConnector &db_con, IOManager &IO_manager)
    : m_client(client), m_client_socketfd(client_socket), m_parent(parent), m_running(true), m_db_con(db_con), m_IO_manager(IO_manager)
{
#ifdef DEBUG
    log("Connection handler constructed successfully");
#endif
}

/**
 * @brief Destroy the Connection Handler:: Connection Handler object
 */
ConnectionHandler::~ConnectionHandler()
{
    delete m_client;
#ifdef DEBUG
    log("Connection handler denstructed successfully");
#endif
}

/**
 * @brief Start the connection thread
 *
 * @return int
 */
int ConnectionHandler::start()
{
    return (pthread_create(&(m_thread), NULL, threadHelper, this) == 0);
}

/**
 * @brief Terminate the thread
 */
void ConnectionHandler::wait()
{
    (void)pthread_join(m_thread, NULL);
}

/**
 * @brief Send data to client
 *
 * @param msg Data to be sent
 * @return int
 */
int ConnectionHandler::send(const std::string &msg) const
{
    if (write(m_client_socketfd, msg.data(), msg.length()) < 0)
    {
#ifdef DEBUG
        log("Connection handler could not write to server");
#endif
        return 1;
    }

    return 0;
}

/**
 * @brief Recieve data from client
 *
 * @param size Size in bytes to recieve
 * @return std::string
 */
std::string ConnectionHandler::recieve(const int size) const
{
    char read_buffer[size];
    bzero(read_buffer, size);

    if (read(m_client_socketfd, read_buffer, size) < 0)
    {
#ifdef DEBUG
        log("Connection handler could not read from server");
#endif
    }

    return std::string(read_buffer);
}

/**
 * @brief Process data recieved by the network
 *
 * @param rec Recieved string
 * @param access_level Return access level
 * @param device_name Return device name
 * @param code Return card code
 */
void ConnectionHandler::processRecievedData(const std::string &rec, int &access_level,
                                            std::string &device_name, std::string &code) const
{
    size_t current_position = 0;
    size_t starting_position = 0;

    // Extract access level
    current_position = rec.find(delimiter, starting_position);
    access_level = std::stoi(rec.substr(starting_position, current_position));
    starting_position = current_position + 1;

    // Extract device_name
    current_position = rec.find(delimiter, starting_position);
    device_name = rec.substr(starting_position, current_position - starting_position);
    starting_position = current_position + 1;

    // Extract code
    code = rec.substr(starting_position, rec.length());
}

/**
 * @brief Main thread loop
 */
void ConnectionHandler::threadLoop()
{
    while (m_running == true)
    {
        int access_level = -1;
        std::string device_name = "NULL";
        std::string code = "NULL";

        std::string rec = recieve(RECIEVE_LENGTH);

        processRecievedData(rec, access_level, device_name, code);

        int status = m_db_con.checkAttempt(code, device_name, access_level);

        if (status != CODE_ACCESS_GRANTED)
        {
            send(std::to_string(CODE_ACCESS_DENIED));
        }
        else
        {
            send(std::to_string(CODE_ACCESS_GRANTED));
        }

        m_running = false;
    }

    m_parent->eraseConnection(this);
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int ConnectionHandler::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}
