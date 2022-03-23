#include <iostream>

#include "ServerSocket.hpp"
#include "IOManager.hpp"

const int SERVER_PORT = 54321;

int main()
{
    IOManager mng("log.txt");
    ServerSocket server(SERVER_PORT, mng);

    std::cout << "Starting server..." << std::endl;

    server.listen();

    return 1;
}
