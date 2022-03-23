#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "ConnectionHandler.hpp"
#include "ServerSocket.hpp"

const int MESSAGE_RECIEVE_LENGTH = 4;

/**
 * @brief Construct a new Connection Handler:: Connection Handler object
 *
 * @param parent ServerSocket instantion that created this thread
 * @param client Client address
 * @param clien_socket Client socket
 * @param IO_manager IO_manager instance responsible for logging
 */
ConnectionHandler::ConnectionHandler(ServerSocket *parent, sockaddr_in *client, int client_socket, IOManager &IO_manager)
    : m_client(client), m_client_socketfd(client_socket), m_parent(parent), m_running(true), m_IO_manager(IO_manager)
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

// TODO delete this
const unsigned char secretCode[] = {0xc4, 0x72, 0x88, 0x03};

static bool isSecretCode(const uint8_t *pbtData, const size_t szBytes)
{
    size_t szPos;
    for (szPos = 0; szPos < szBytes; szPos++)
    {
        if (szPos > (sizeof(secretCode) / sizeof(char)))
            break;
        if (pbtData[szPos] != secretCode[szPos])
            return false;
    }
    return true;
}

/**
 * @brief Main thread loop
 */
void ConnectionHandler::threadLoop()
{
    while (m_running == true)
    {
        std::string rec = recieve(MESSAGE_RECIEVE_LENGTH);

        send(std::to_string(isSecretCode((uint8_t *)rec.c_str(), rec.length())));

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
