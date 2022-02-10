#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <fstream>
#include <string>
/**
 * @brief IOManager is responsible for all the input/output to files
 */
class IOManager
{
private:
    std::fstream m_stream;
    const std::string m_filename;
    pthread_mutex_t m_lock;

public:
    IOManager(const std::string &filename);
    ~IOManager();

    int write(const std::string &data);
    int read(std::string &result);
};

#endif // IOManager
