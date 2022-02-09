// #include <iostream>
// #include <unistd.h>

// #include "GPIO.hpp"
// #include "IOManager.hpp"
// #include "ClientSocket.hpp"

int main()
{
    // IOManager mng("log.txt");

    // GPIO lock(16, &mng);
    // std::cout << lock.getDirection() << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(5);

    // std::cout << lock.setDirection(GPIO_DIRECTION::OUTPUT) << std::endl;
    // std::cout << lock.setValue(GPIO_VALUE::HIGH) << std::endl;
    // std::cout << lock.getDirection() << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(10);

    // std::cout << lock.setDirection(GPIO_DIRECTION::INPUT) << std::endl;
    // std::cout << lock.setValue(GPIO_VALUE::HIGH) << std::endl;
    // std::cout << lock.getDirection() << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(10);

    // std::cout << lock.setDirection(GPIO_DIRECTION::OUTPUT) << std::endl;
    // std::cout << lock.setValue(GPIO_VALUE::HIGH) << std::endl;
    // std::cout << lock.getDirection() << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(10);

    // ClientSocket inst("www.google.com", 80, &mng);

    // inst.connectToServer();

    // std::string message("GET / HTTP/1.1\n\n");

    // std::cout << "Sending [" << message << "]" << std::endl;

    // inst.send(message);

    // std::string result = inst.recieve(1024);

    // std::cout << "Recieved [" << result << "]" << std::endl;

    // inst.disconnectFromServer();

    return 0;
}
