#include "ServerSocket.hpp"
#include "ConnectionHandler.hpp"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>

const int CONNECTIONS_QUEUE = 5;

/**
 * @brief Construct a new Server Socket:: Server Socket object
 *
 * @param port_number Server port
 * @param IO_manager IO_manager instance responsible for logging
 */
ServerSocket::ServerSocket(const int &port_number, DatabaseConnector &db_con, IOManager &IO_manager)
    : m_socketfd(-1), m_port_number(port_number),
      m_connections(std::vector<ConnectionHandler *>()),
      m_db_con(db_con), m_IO_manager(IO_manager)
{
    memset((char *)&m_server_address, 0, sizeof(m_server_address));
}
/**
 * @brief Destroy the Server Socket:: Server Socket object
 * closes server and client sockets
 */
ServerSocket::~ServerSocket()
{
    close(m_socketfd);
}

/**
 * @brief Start listening for client connections
 * and map them to ConnectionHandlers
 *
 * @return int
 */
int ServerSocket::listen()
{
    m_socketfd = socket(AF_INET, SOCK_STREAM, 0); // create socket

    if (m_socketfd < 0)
    {
#ifdef DEBUG
        log("Cannot create server socket");
#endif
        return 1;
    }

    // setup socket
    m_server_address.sin_family = AF_INET;
    m_server_address.sin_addr.s_addr = INADDR_ANY;
    m_server_address.sin_port = htons(m_port_number);

    if (bind(m_socketfd, (sockaddr *)&m_server_address, sizeof(m_server_address)) < 0)
    {
#ifdef DEBUG
        log("Cannot bind server socket");
#endif
        return 1;
    }

    // start socket
    ::listen(m_socketfd, CONNECTIONS_QUEUE);

    while (true)
    {
        sockaddr_in *temp_socket_addr = new sockaddr_in();
        socklen_t temp_socket_length = sizeof(*temp_socket_addr);

        // accept socket connection
        int temp_socketfd = accept(m_socketfd, (sockaddr *)temp_socket_addr, &temp_socket_length);
        if (temp_socketfd < 0)
        {
#ifdef DEBUG
            log("Cannot bind client socket to new thread");
#endif
            delete temp_socket_addr; // TODO ?
            return 1;
        }
        else
        {
            // create and bind new connection to the connection handler vector
            ConnectionHandler *new_connection = new ConnectionHandler(this, temp_socket_addr, temp_socketfd, m_db_con, m_IO_manager);
            m_connections.push_back(new_connection);
            new_connection->start();
        }
    }
    return 1;
}

/**
 * @brief Disconnect client
 *
 * @param connection connection handler instance to disconnect
 */
void ServerSocket::eraseConnection(ConnectionHandler *connection)
{
    std::vector<ConnectionHandler *>::iterator it;
    for (it = m_connections.begin(); it <= m_connections.end(); it++)
    {
        if (*it == connection)
        {
            m_connections.erase(it);
#ifdef DEBUG
            log("Connection deleted");
#endif
        }
    }
    delete connection;
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int ServerSocket::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}
