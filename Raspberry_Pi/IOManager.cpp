#include "IOManager.hpp"
#include <fstream>
#include <string>

/**
 * @brief Construct a new IOManager::IOManager object
 *
 * @param file Name of file to be written or read from
 */
IOManager::IOManager(const std::string &file) : available(IO_AVAILABLE), stream(), filename(file) {}

/**
 * @brief Thread safe method responsible for writing data to file
 *
 * @param data String with data to be written
 * @return int
 */
int IOManager::write(const std::string &data)
{
    if (available == IO_AVAILABLE) // check for concurrency
    {
        available = IO_BUSY; // disable interrupts

        stream.open(filename, std::fstream::out | std::fstream::app); // open stream
        stream << data << std::endl;                                  // write to stream
        stream.close();                                               // close stream

        available = IO_AVAILABLE; // enable interrupts
    }

    return available;
}

/**
 * @brief Thread safe method responsible for reading data to file
 *
 * @param result String data passed by reference. The result is saved in it
 * @return int
 */
int IOManager::read(std::string &result)
{
    if (available == IO_AVAILABLE)
    {
        available = IO_BUSY;                     // disable interrupts
        stream.open(filename, std::fstream::in); // open stream
        std::string buffer = "";                 // create buffer variable
        while (getline(stream, buffer))          // get each line
        {
            result.append(buffer + "\n"); // append each line to buffer
        }
        available = IO_AVAILABLE; // enable interrupts
    }

    return available;
}