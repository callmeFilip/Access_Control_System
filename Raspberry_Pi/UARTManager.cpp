#include <unistd.h>
#include <string>
#include <termios.h>
#include <fcntl.h>

#include "UARTManager.hpp"

/**
 * @brief Construct a new UARTManager::UARTManager object
 *
 * @param bus_path File path to device
 * @param IO_manager IO_manager instance responsible for logging
 */
UARTManager::UARTManager(const std::string &bus_path, IOManager *IO_manager)
    : m_bus_file_descriptor(-1), m_IO_manager(IO_manager), m_is_open(false)
{
    if (openConnection(bus_path) == 0)
    {
        log("UARTManager constructed successfully");
    }
    else
    {
        log("UARTManager constructed unsuccessfully");
    }
}

/**
 * @brief Destroy the UARTManager::UARTManager object
 * Calls close connection and logs
 */
UARTManager::~UARTManager()
{
    if (closeConnection() == 0)
    {
        log("UARTManager deconstructed successfully");
    }
    else
    {
        log("UARTManager deconstructed unsuccessfully");
    }
}

/**
 * @brief Open connection to UART device
 *
 * @param bus_path File path to device
 * @return int
 */
int UARTManager::openConnection(const std::string &bus_path)
{
    if (m_is_open)
    {
        log("UART connection is already opened");
        return 1;
    }

    // Open file descriptor with ReadWrite, Dont make it controlling terminal, Non-blocking mode
    m_bus_file_descriptor = open(bus_path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (m_bus_file_descriptor < 0)
    {
        log("UARTManager failed to open the device");
        return 1;
    }

    struct termios options;
    tcgetattr(m_bus_file_descriptor, &options); // Copy file options

    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL; // Baudrate 9600(NPN532 default), 8 bit word, enable read, disable model-specific signal, disable parity
    // options.c_cflag &= ~PARENB; // disable paring bit
    options.c_iflag = IGNPAR | ICRNL; // Ignore framing and parity errors, Translate carriage
                                      // return to newline on input

    tcflush(m_bus_file_descriptor, TCIFLUSH);            // Flush recieved data that is not read
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);            // Make reads non-blocking
    tcsetattr(m_bus_file_descriptor, TCSANOW, &options); // Set attributes immediately

    m_is_open = true;

    return 0;
}

/**
 * @brief Close connection to UART device
 *
 * @return int
 */
int UARTManager::closeConnection()
{
    if (!m_is_open)
    {
        log("UART connection is not opened");
        return 1;
    }

    if (close(m_bus_file_descriptor) < 0)
    {
        log("UART connection is not closed successfully");
        return 1;
    }

    m_is_open = false;

    return 0;
}

/**
 * @brief Write to bus file descriptor
 *
 * @param data Data to write
 * @param length Length of data
 * @return int
 */
int UARTManager::transmit(const char *data, size_t length = 1) const
{
    if (!m_is_open)
    {
        log("UART is not connected(transmit)");
        return 1;
    }

    if (write(m_bus_file_descriptor, data, length) < 0)
    {
        log("UART failed to write data");
        return 1;
    }

    // write(m_bus_file_descriptor, "\n\r", 2);

    // log("[TODO DELETE THIS] msg sent");

    return 0;
}

/**
 * @brief Read data from file descriptor and return it
 *
 * @param result Saves result to buffer
 * @param length Size of bytes to recieve
 * @return int
 */
int UARTManager::recieve(char *result, size_t length = 1) const
{
    if (!m_is_open)
    {
        log("UART is not connected(recieve)");
        return 1;
    }

    if (read(m_bus_file_descriptor, result, length) < 0)
    {
        log("UART failed to read data");

        return 1;
    }

    // log("[TODO DELETE THIS] msg recieved");

    return 0;
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int UARTManager::log(const std::string &msg) const
{
    return m_IO_manager->write(msg);
}
