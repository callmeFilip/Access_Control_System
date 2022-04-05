#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "GPIO.hpp"
#include "IOManager.hpp"
#include "ClientSocket.hpp"
// #include "UARTManager.hpp"
#include "PN532.hpp"
#include "LED.hpp"
#include "Lock.hpp"

const int GPIO_LOCK = 12;
const int GPIO_LED_GREEN = 8;
const int GPIO_LED_RED = 25;

const int LOCK_TIME = 5; // seconds
const int DELAY = 2;     // seconds

#define SERVER_NAME "192.168.0.105"
const int SERVER_PORT = 54321;
const int MESSAGE_TRANSMIT_LENGTH = 64;
const int MESSAGE_RECIEVE_LENGTH = 8;

const int CODE_ACCESS_GRANTED = 1;
const int CODE_ACCESS_DENIED = 0;

const int DEVICE_ACCESS_LEVEL = 2;
const std::string DEVICE_NAME = "Raspberry Pi #1";

// Initialize log output
IOManager mng("log.txt");

// Initialize lock
Lock lock(GPIO_LOCK, mng);

// Initialize LEDs
LED green(GPIO_LED_GREEN, mng, true);
LED red(GPIO_LED_RED, mng, true);

// Initialize PN532
PN532 sensor(mng, NULL);

const nfc_modulation nmMifare =
    {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106,
};

nfc_target target; // buffer device

// Initialize TCP/IP connection
ClientSocket connection(SERVER_NAME, SERVER_PORT, mng);

void log(const std::string &msg)
{
    mng.write(msg);
}

#ifdef DEBUG
static void print_hex(const uint8_t *pbtData, const size_t szBytes)
{
    size_t szPos;
    for (szPos = 0; szPos < szBytes; szPos++)
    {
        printf("%02x  ", pbtData[szPos]);
    }
    printf("\n");
}
#endif

void accessGrant()
{
    lock.setLock(HIGH);

    if (lock.getValue() == HIGH)
    {
        green.setLed(HIGH);

        sleep(LOCK_TIME);

        green.setLed(LOW);
    }
    else
    {
#ifdef DEBUG
        log("Lock sequence failure");
#endif
    }

    lock.setLock(LOW);
}

void accessDeny()
{
    red.setLed(LOW);
    sleep(1);
    red.setLed(HIGH);
    sleep(1);
    red.setLed(LOW);
    sleep(1);
    red.setLed(HIGH);
    sleep(1);
    red.setLed(LOW);
    sleep(1);
    red.setLed(HIGH);
}

void accessNoResponse()
{
    red.setLed(LOW);
    sleep(LOCK_TIME);
    red.setLed(HIGH);
}

/**
 * @brief Convert hex data to string
 *
 * @param data Pointer to uint8_t array
 * @param size Size of the array
 * @return std::string
 */
std::string hexToString(const uint8_t *data, const size_t size)
{
    std::stringstream ss;

    ss << std::hex << std::setfill('0');

    for (size_t i = 0; i < size; i++)
    {
        ss << std::hex << std::setw(2) << static_cast<int>(data[i]);
    }

    return ss.str();
}

int main()
{
    int server_reply = -1;

    red.setLed(HIGH); // System is initialized and ready

    while (true)
    {
        sensor.poll(nmMifare, target);

#ifdef DEBUG
        print_hex(target.nti.nai.abtUid, target.nti.nai.szUidLen);
#endif
        if (connection.connectToServer() == 0)
        {
            connection.send(std::to_string(DEVICE_ACCESS_LEVEL) + "&" + DEVICE_NAME + "&" + hexToString(target.nti.nai.abtUid, target.nti.nai.szUidLen));

            server_reply = std::stoi(connection.recieve(MESSAGE_RECIEVE_LENGTH));
            // server_reply = isSecretCode(target.nti.nai.abtUid, target.nti.nai.szUidLen);

            if (server_reply == CODE_ACCESS_GRANTED)
            {
                accessGrant();
#ifdef DEBUG
                log("Access Granted");
#endif
            }
            else if (server_reply == CODE_ACCESS_DENIED)
            {
                accessDeny();
#ifdef DEBUG
                log("Access Denied");
#endif
            }
            else
            {
                accessNoResponse();
#ifdef DEBUG
                log("No responce");
#endif
            }
            connection.disconnectFromServer();
        }
        else
        {
            accessNoResponse();
#ifdef DEBUG
            log("Connection to server failed");
#endif
        }

        sleep(DELAY);
    }

    return 1;
}
