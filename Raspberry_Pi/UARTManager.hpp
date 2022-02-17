#ifndef UARTMANAGER_H
#define UARTMANAGER_H

#include <string>

#include "IOManager.hpp"

class UARTManager
{
private:
    int m_bus_file_descriptor;
    IOManager &m_IO_manager;
    bool m_is_open;

    int log(const std::string &msg) const;
    int openConnection(const std::string &bus_path);
    int closeConnection();

public:
    UARTManager(const std::string &bus_path, IOManager &IO_manager);
    ~UARTManager();

    int transmit(const char *data, size_t length) const;
    int recieve(char *result, size_t length) const;

    bool isOpen() const { return m_is_open; }
};

#endif // UARTManager
