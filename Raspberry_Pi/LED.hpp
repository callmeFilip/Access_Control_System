#ifndef LED_H
#define LED_H

#include "GPIO.hpp"
#include "IOManager.hpp"

class LED : public GPIO
{
    const bool m_inverted;

public:
    LED(int gpio_number, IOManager &IO_manager, bool inverted);
    ~LED();
    int setLed(const GPIO_VALUE &value);
};

#endif // LED