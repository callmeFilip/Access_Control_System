#include <fstream>
#include <string>
/**
 * @brief IOManager is responsible for all the input/output to files
 */
static const int IO_AVAILABLE = 1;
static const int IO_BUSY = 0;
class IOManager
{
private:
    int available;
    std::fstream stream;
    const std::string filename;

public:
    IOManager(const std::string &file) : available(IO_AVAILABLE), stream(), filename(file) {}
    int write(const std::string &data);
    int read(std::string &result);
}; // IOManager
