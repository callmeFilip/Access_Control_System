#include "ClientSocket.hpp"
#include <string.h>

const int CLIENT_SOCKET_DEFAULT_RECIEVE_SIZE = 1024;

/**
 * @brief Construct a new ClientSocket::ClientSocket object
 *
 * @param server_name Name of server (could be ip)
 * @param port_number Port number
 * @param IO_manager IO_manager instance responsible for logging
 */
ClientSocket::ClientSocket(const std::string &server_name,
                           int port_number, IOManager &IO_manager)
    : m_socketfd(-1), m_port_number(port_number),
      m_server(NULL), m_server_name(server_name),
      m_is_connected(false), m_IO_manager(IO_manager)

{
    log("Client socket constructed successfully"); // TODO remove log here
}

/**
 * @brief Destroy the ClientSocket::ClientSocket object
 * Calls disconnect from server and logs
 */
ClientSocket::~ClientSocket()
{
    if (m_is_connected == true)
    {
        disconnectFromServer();
    }

    log("Client socket destructed successfully");
}

/**
 * @brief Create socket, get server host entry, connect to server
 *
 * @return int
 */
int ClientSocket::connectToServer()
{
    m_socketfd = socket(AF_INET, SOCK_STREAM, 0); // create socket

    if (m_socketfd < 0)
    {
        log("Cannot create client socket");
        return 1;
    }

    m_server = gethostbyname(m_server_name.data()); // get server address from DNS server

    if (m_server == NULL)
    {
        log("Client socket could not find host");
        return 1;
    }

    bzero((char *)&m_server_address,
          sizeof(m_server_address)); // assign default server address

    m_server_address.sin_family = AF_INET; // set address family to INET

    bcopy((char *)m_server->h_addr,
          (char *)&m_server_address.sin_addr.s_addr,
          m_server->h_length); // copy server address from DNS database to sockaddr struct

    m_server_address.sin_port = htons(m_port_number); // transform port number to TCP/IP suitable byte order (INET family)

    log("Client socket initialized successfully");

    if (connect(m_socketfd, (sockaddr *)&m_server_address, sizeof(m_server_address)) < 0)
    {
        log("Client socket could not connect to the server");
        return 1;
    }

    m_is_connected = true;
    log("Successfully connected to the server");

    return 0;
}

/**
 * @brief Disconnect from server and close socket
 *
 * @return int
 */
int ClientSocket::disconnectFromServer()
{
    if (m_is_connected)
    {
        m_is_connected = false;

        close(m_socketfd);

        log("Successfully disconnected from the server");

        return 0;
    }
    else
    {
        log("Client socket is already disconnected");
        return 1;
    }
}

/**
 * @brief Write to socket
 *
 * @param msg Message to transmit
 * @return int
 */
int ClientSocket::send(const std::string &msg) const
{
    if (!m_is_connected)
    {
        log("Client socket not connected");
        return 1;
    }

    if (write(m_socketfd, msg.c_str(), msg.length()) < 0)
    {
        log("Client socket could not send a message");
        return 1;
    }

    return 0;
}

/**
 * @brief Read from file descriptor and return the data
 *
 * @param size Size of bytes to read from file descriptor
 * @return std::string
 */
std::string ClientSocket::recieve(const int size = CLIENT_SOCKET_DEFAULT_RECIEVE_SIZE) const
{
    if (!m_is_connected)
    {
        log("Client socket not connected");
        return "ERROR";
    }

    char buffer[size];
    bzero(buffer, size);

    if (read(m_socketfd, buffer, sizeof(buffer)) < 0)
    {
        log("Client socket could not read a message");
        return "ERROR";
    }

    return std::string(buffer);
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int ClientSocket::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}
