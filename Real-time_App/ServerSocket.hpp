#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include "ConnectionHandler.hpp"
#include "IOManager.hpp"

class ServerSocket
{
private:
    int m_socketfd;
    int m_port_number;
    sockaddr_in m_server_address;
    sockaddr_in m_client_address;
    std::vector<ConnectionHandler *> m_connections;
    IOManager &m_IO_manager;

    int log(const std::string &msg) const;

public:
    ServerSocket(const int &port_number, IOManager &IO_manager);
    ~ServerSocket();

    int listen();

    void eraseConnection(ConnectionHandler *connections);
};

#endif // ServerSocket
