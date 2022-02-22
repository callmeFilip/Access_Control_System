#include "LED.hpp"

/**
 * @brief Construct a new LED::LED object
 * Sets GPIO direction as output and sets LED to OFF
 *
 * @param gpio_number Number of gpio
 * @param IO_manager IO_manager instance responsible for logging
 * @param inverted Is LED connected inverted
 */
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

/**
 * @brief Destroy the LED::LED object
 *
 */
LED::~LED()
{
    log("LED unitilized");
}

/**
 * @brief Turns LED OFF or ON
 *
 * @param value GPIO Value for LED
 * @return int
 */
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
