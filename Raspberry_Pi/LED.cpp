#include "LED.hpp"

LED::LED(int gpio_number, IOManager &IO_manager, bool inverted)
    : GPIO(gpio_number, IO_manager), m_inverted(inverted)
{
    GPIO::setDirection(OUTPUT);

    if (m_inverted)
    {
        GPIO::setValue(HIGH);
    }
    else
    {
        GPIO::setValue(LOW);
    }

    log("LED initialized");
}

LED::~LED()
{
    log("LED unitilized");
}

int LED::setLed(const GPIO_VALUE &value)
{
    if (m_inverted)
    {
        if (value == HIGH)
        {
            return setValue(LOW);
        }
        else if (value == LOW)
        {
            return setValue(HIGH);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if (value == HIGH)
        {
            return setValue(HIGH);
        }
        else if (value == LOW)
        {
            return setValue(LOW);
        }
        else
        {
            return 1;
        }
    }
}
