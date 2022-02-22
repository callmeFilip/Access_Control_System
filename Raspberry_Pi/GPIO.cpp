#include <fstream>

#include "GPIO.hpp"
#include "GPIO_config.hpp"

/**
 * @brief Construct a new GPIO::GPIO object
 * Calls export and logs the results. Default state
 * of GPIO is input
 *
 * @param gpio_number Number of gpio
 * @param direction Direction of gpio
 * @param IO_manager IO_manager instance responsible for logging
 */
GPIO::GPIO(int gpio_number, IOManager &IO_manager)
    : m_gpio_number(gpio_number), m_direction(GPIO_DIRECTION::GPIO_DIRECTION_UNKNOWN),
      m_value(GPIO_VALUE::GPIO_VALUE_UNKNOWN), m_IO_manager(IO_manager)
{
    if (exportGPIO() != 0)
    {
        log("GPIO " + std::to_string(gpio_number) + " Failed to export!");
        exit(1); // cannot exit because allocated memory will not be freed! ???? or is it # TODO
    }
    log("GPIO " + std::to_string(gpio_number) + " exported successffully!");

    m_direction = getDirection();
    m_value = getValue();
}

/**
 * @brief Destroy the GPIO::GPIO object
 * Calls unexport and logs the results
 */
GPIO::~GPIO()
{
    setDirection(GPIO_DIRECTION::INPUT); // unexport GPIO as INPUT for safety reasons

    if (unexportGPIO() != 0)
    {
        log("GPIO " + std::to_string(m_gpio_number) + " Failed to unexport!");
        exit(1); // cannot exit because allocated memory will not be freed! ???? or is it # TODO
    }
    log("GPIO " + std::to_string(m_gpio_number) + " unexported successffully!");
}

/**
 * @brief Exports the GPIO pin via Linux filesystem
 *
 * @return int
 */
int GPIO::exportGPIO() const
{
    std::ofstream out(GPIO_EXPORT_PATH);
    if (out.is_open() != true)
    {
        log("Could not open : " GPIO_EXPORT_PATH);
        return 1;
    }

    out << m_gpio_number << std::endl;
    out.close();

    return 0;
}

/**
 * @brief Unexports the GPIO pin via Linux filesystem
 *
 * @return int
 */
int GPIO::unexportGPIO() const
{
    std::ofstream out(GPIO_UNEXPORT_PATH);

    if (out.is_open() != true)
    {
        log("Could not open : " GPIO_UNEXPORT_PATH);
        return 1;
    }

    out << m_gpio_number << std::endl;
    out.close();

    return 0;
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int GPIO::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}

/**
 * @brief Set direction to GPIO pin
 *
 * @param direction GPIO direction
 * @return int
 */
int GPIO::setDirection(const GPIO_DIRECTION &direction)
{
    if (direction != GPIO_DIRECTION::INPUT && direction != GPIO_DIRECTION::OUTPUT)
    {
        log("Invalid direction");
        return 1;
    }

    m_direction = direction;

    return writeDirection();
}

/**
 * @brief Set value to GPIO pin
 *
 * @param value GPIO value
 * @return int
 */
int GPIO::setValue(const GPIO_VALUE &value)
{
    if (m_direction != GPIO_DIRECTION::OUTPUT)
    {
        log("GPIO " + std::to_string(m_gpio_number) + " direction not set to OUTPUT");
        return 1;
    }

    if (value != GPIO_VALUE::HIGH && value != GPIO_VALUE::LOW)
    {
        log("Invalid value");
        return 1;
    }

    m_value = value;

    return writeValue();
}

/**
 * @brief Write direction to GPIO pin via Linux filesystem
 *
 * @return int
 */
int GPIO::writeDirection() const
{
    std::ofstream out(GPIO_DIRECTION_PATH(m_gpio_number));
    if (out.is_open() != true)
    {
        log("Could not open : " GPIO_DIRECTION_PATH(m_gpio_number));
        return 1;
    }
    switch (m_direction)
    {
    case OUTPUT:
        out << "out" << std::endl;
        break;

    case INPUT:
        out << "in" << std::endl;
        break;

    default: // probably will never come to this point
        log("Error has occurred in direction change");
        out.close();
        return 1;

        break;
    }

    out.close();

    return 0;
}

/**
 * @brief Write value to GPIO pin via Linux filesystem
 *
 * @return int
 */
int GPIO::writeValue() const
{
    std::ofstream out(GPIO_VALUE_PATH(m_gpio_number));

    if (out.is_open() != true)
    {
        log("Could not open : " GPIO_VALUE_PATH(m_gpio_number));
        return 1;
    }

    out << m_value << std::endl;

    out.close();

    return 0;
}

/**
 * @brief Retrieve GPIO direction
 *
 * @return GPIO_DIRECTION
 */
GPIO_DIRECTION GPIO::getDirection()
{
    std::ifstream in(GPIO_DIRECTION_PATH(m_gpio_number));

    if (in.is_open() != true)
    {
        log("Could not open : " GPIO_DIRECTION_PATH(m_gpio_number));
        m_direction = GPIO_DIRECTION::GPIO_DIRECTION_UNKNOWN;
        return m_direction;
    }

    std::string gpio_direction_str = "";
    std::getline(in, gpio_direction_str);

    if (gpio_direction_str == "in")
    {
        m_direction = GPIO_DIRECTION::INPUT;
    }
    else if (gpio_direction_str == "out")
    {
        m_direction = GPIO_DIRECTION::OUTPUT;
    }
    else
    {
        m_direction = GPIO_DIRECTION::GPIO_DIRECTION_UNKNOWN;
        log("Unknown GPIO direction");
    }

    return m_direction;
}

/**
 * @brief Retrieve GPIO value
 *
 * @return GPIO_VALUE
 */
GPIO_VALUE GPIO::getValue()
{
    std::ifstream in(GPIO_VALUE_PATH(m_gpio_number));

    if (in.is_open() != true)
    {
        log("Could not open : " GPIO_VALUE_PATH(m_gpio_number));
        m_value = GPIO_VALUE::GPIO_VALUE_UNKNOWN;
        return m_value;
    }

    std::string gpio_value_str = "";
    std::getline(in, gpio_value_str);

    if (gpio_value_str == "0")
    {
        m_value = GPIO_VALUE::LOW;
    }
    else if (gpio_value_str == "1")
    {
        m_value = GPIO_VALUE::HIGH;
    }
    else
    {
        m_value = GPIO_VALUE::GPIO_VALUE_UNKNOWN;
        log("Unknown GPIO value");
    }

    return m_value;
}
