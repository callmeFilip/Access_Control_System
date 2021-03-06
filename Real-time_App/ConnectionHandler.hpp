#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

#include "IOManager.hpp"
#include "DatabaseConnector.hpp"

class ServerSocket;

class ConnectionHandler
{
private:
    sockaddr_in *m_client;
    int m_client_socketfd;
    pthread_t m_thread;
    ServerSocket *m_parent;
    bool m_running;
    DatabaseConnector &m_db_con;
    IOManager &m_IO_manager;

    int log(const std::string &msg) const;

    void processRecievedData(const std::string &rec, int &access_level, std::string &device_name, std::string &code) const;

    void threadLoop();

    static void *threadHelper(void *handler)
    {
        ((ConnectionHandler *)handler)->threadLoop();
        return NULL;
    }

public:
    ConnectionHandler(ServerSocket *parent, sockaddr_in *client, int client_socket, DatabaseConnector &db_con, IOManager &IO_manager);
    ~ConnectionHandler();

    int start();
    void wait();
    void stop() { m_running = false; }

    int send(const std::string &msg) const;
    std::string recieve(const int size) const;
};

#endif // ConnectionHandler
