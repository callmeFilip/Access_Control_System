// #include <iostream>
// #include <unistd.h>
// #include <stdlib.h>

// #include "GPIO.hpp"
// #include "IOManager.hpp"
// #include "ClientSocket.hpp"
// #include "UARTManager.hpp"
// #include "PN532.hpp"
// #include "LED.hpp"
// #include "Lock.hpp"

// char secretCode[] = {0xc4, 0x72, 0x88, 0x03};

// static void print_hex(const uint8_t *pbtData, const size_t szBytes)
// {
//     size_t szPos;
//     for (szPos = 0; szPos < szBytes; szPos++)
//     {
//         printf("%02x  ", pbtData[szPos]);
//     }
//     printf("\n");
// }

// static bool isSecretCode(const uint8_t *pbtData, const size_t szBytes)
// {
//     size_t szPos;
//     for (szPos = 0; szPos < szBytes; szPos++)
//     {
//         if (szPos > (sizeof(secretCode) / sizeof(char)))
//             break;
//         if (pbtData[szPos] != secretCode[szPos])
//             return false;
//     }
//     return true;
// }

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

    // UARTManager uart("/dev/ttyAMA0", &mng);
    // char *result = new char[6];
    // for (size_t i = 0; i < 5; i++)
    // {
    //     result[i] = '/';
    // }

    // result[5] = '\0';

    // uart.transmit("hello", 6);
    // uart.recieve(result, 6);

    // for (size_t i = 0; i < 5; i++)
    // {
    //     std::cout << (int)result[i] << std::endl;
    // }
    // std::cout << "1" << std::endl;

    // delete[] result;

    // PN532 sensor(mng, NULL);

    // const nfc_modulation nmMifare =
    //     {
    //         .nmt = NMT_ISO14443A,
    //         .nbr = NBR_106,
    //     };

    // nfc_target target;

    // sensor.poll(nmMifare, target);

    // print_hex(target.nti.nai.abtUid, target.nti.nai.szUidLen);
    // if (isSecretCode(target.nti.nai.abtUid, target.nti.nai.szUidLen))
    // {
    //     printf("Yes\n");
    // }
    // else
    // {
    //     printf("No\n");
    // }

    // LED led(16, mng, true);

    // std::cout << led.setLed(HIGH) << std::endl;
    // std::cout << led.getValue() << std::endl;

    // sleep(10);

    // std::cout << led.setLed(LOW) << std::endl;
    // std::cout << led.getValue() << std::endl;

    // sleep(10);

    // Lock lock(20, mng);

    // std::cout << lock.setLock(HIGH) << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(10);

    // std::cout << lock.setLock(LOW) << std::endl;
    // std::cout << lock.getValue() << std::endl;

    // sleep(10);

    return 0;
}
