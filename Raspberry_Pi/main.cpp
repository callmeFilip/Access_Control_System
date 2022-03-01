#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>

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

const int LOCK_TIME = 7; // seconds

#define SERVER_NAME "www.google.com"
const int SERVER_PORT = 80;

const int CODE_ACCESS_GRANTED = 1;
const int CODE_ACCESS_DENIED = 0;

char secretCode[] = {0xc4, 0x72, 0x88, 0x03};

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

static bool isSecretCode(const uint8_t *pbtData, const size_t szBytes)
{
    size_t szPos;
    for (szPos = 0; szPos < szBytes; szPos++)
    {
        if (szPos > (sizeof(secretCode) / sizeof(char)))
            break;
        if (pbtData[szPos] != secretCode[szPos])
            return false;
    }
    return true;
}

void accessGrant()
{
    lock.setLock(HIGH);

    if (lock.getValue() == HIGH)
    {
        green.setLed(HIGH);

        sleep(LOCK_TIME);

        green.setLed(LOW);
#ifdef DEBUG
        log("Lock sequence successfull");
#endif
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

#ifdef DEBUG
    log("Access denied");
#endif
}

void accessNoResponse()
{
    red.setLed(LOW);
    sleep(LOCK_TIME);
    red.setLed(HIGH);

#ifdef DEBUG
    log("Server did not respond");
#endif
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
            connection.send(std::string((char *)target.nti.nai.abtUid));
            // server_reply = std::stoi(connection.recieve(1024));
            server_reply = isSecretCode(target.nti.nai.abtUid, target.nti.nai.szUidLen);

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
            // delete

            log("Connection to server failed");
        }

        sleep(LOCK_TIME);
    }

    return 1;
}
