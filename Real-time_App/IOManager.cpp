#include "IOManager.hpp"
#include <fstream>
#include <string>

#include <iostream>

/**
 * @brief Construct a new IOManager::IOManager object
 * Init mutex
 * @param file Name of file to be written or read from
 */
IOManager::IOManager(const std::string &filename) : m_stream(), m_filename(filename)
{
    pthread_mutex_init(&m_lock, NULL);
}

/**
 * @brief Destroy the IOManager::IOManager object
 * Destroy mutex
 */
IOManager::~IOManager()
{
    pthread_mutex_destroy(&m_lock);
}

/**
 * @brief pthread safe method responsible for writing data to file
 *
 * @param data String with data to be written
 * @return int
 */
int IOManager::write(const std::string &data)
{
    pthread_mutex_lock(&m_lock); // lock thread

    m_stream.open(m_filename, std::fstream::out | std::fstream::app); // open stream
    m_stream << data << std::endl;                                    // write to stream
    m_stream.close();                                                 // close stream

    pthread_mutex_unlock(&m_lock); // unlock thread

    return 1;
}

/**
 * @brief pthread safe method responsible for reading data to file
 *
 * @param result String data passed by reference. The result is saved in it
 * @return int
 */
int IOManager::read(std::string &result)
{
    pthread_mutex_lock(&m_lock); // lock thread

    m_stream.open(m_filename, std::fstream::in); // open stream
    std::string buffer = "";                     // create buffer variable
    while (getline(m_stream, buffer))            // get each line
    {
        result.append(buffer + "\n"); // append each line to buffer
    }
    m_stream.close(); // close stream

    pthread_mutex_unlock(&m_lock); // unlock thread

    return 1;
}