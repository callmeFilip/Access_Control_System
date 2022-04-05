#include <iostream>
#include "ServerSocket.hpp"
#include "IOManager.hpp"
#include "DatabaseConnector.hpp"

const int SERVER_PORT = 54321;

int main()
{
    IOManager mng("log.txt");

    DatabaseConnector dbcon("mysqlx://root:root1234@localhost:33060/Access_Control_System", mng);

    ServerSocket server(SERVER_PORT, dbcon, mng);

    std::cout << "Starting server..." << std::endl;

    server.listen();

    return 1;
}
