#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>

#include "IOManager.hpp"

class ClientSocket
{
private:
    int m_socketfd;
    int m_port_number;
    sockaddr_in m_server_address;
    hostent *m_server;
    std::string m_server_name;
    bool m_is_connected;
    IOManager &m_IO_manager;

    int log(const std::string &msg) const;

public:
    ClientSocket(const std::string &server_name, int port_number, IOManager &IO_manager);
    ~ClientSocket();

    int connectToServer();
    int disconnectFromServer();

    int send(const std::string &msg) const;
    std::string recieve(const int size) const;

    bool isConnected() const { return m_is_connected; }
};

#endif // ClientSocket