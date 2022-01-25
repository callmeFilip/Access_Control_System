#include "IOManager.hpp"
#include <fstream>
#include <string>

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