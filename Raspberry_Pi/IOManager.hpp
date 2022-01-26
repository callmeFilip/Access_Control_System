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
    std::fstream stream;
    const std::string filename;
    pthread_mutex_t lock;

public:
    IOManager(const std::string &file);
    ~IOManager();

    int write(const std::string &data);
    int read(std::string &result);
};

#endif // IOManager
