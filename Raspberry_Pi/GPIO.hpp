#ifndef GPIO_H
#define GPIO_H

#include "IOManager.hpp"

enum GPIO_DIRECTION
{
    INPUT = 0,
    OUTPUT = 1,
    GPIO_DIRECTION_UNKNOWN = 2
};

enum GPIO_VALUE
{
    LOW = 0,
    HIGH = 1,
    GPIO_VALUE_UNKNOWN = 2
};

/**
 * @brief GPIO is an abstract class, responsible for
 * communication with physical GPIOs on OS level
 */
class GPIO
{
private:
    const int m_gpio_number;
    GPIO_DIRECTION m_direction;
    GPIO_VALUE m_value;
    IOManager &m_IO_manager;

    int exportGPIO() const;
    int unexportGPIO() const;
    int writeDirection() const;
    int writeValue() const;

protected:
    // TODO INIT GPIO CONSTRUCTOR AND DESTRUCTOR IN LOCK AND LED!!!
    GPIO(int gpio_number, IOManager &IO_manager);
    virtual ~GPIO() = 0; // pure destructor

    int setDirection(const GPIO_DIRECTION &);
    GPIO_DIRECTION getDirection();

    int setValue(const GPIO_VALUE &);

    int getNumber() const { return m_gpio_number; }

    virtual int log(const std::string &msg) const;

public:
    GPIO_VALUE getValue();
};

#endif // GPIO