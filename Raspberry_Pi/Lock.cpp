#include "Lock.hpp"

/**
 * @brief Construct a new Lock:: Lock object
 * Set GPIO value to OUTPUT and value to LOW
 *
 * @param gpio_number
 * @param IO_manager
 */
Lock::Lock(int gpio_number, IOManager &IO_manager)
    : GPIO(gpio_number, IO_manager)
{
    GPIO::setDirection(OUTPUT);
    GPIO::setValue(LOW);
#ifdef DEBUG
    log("Lock initialized");
#endif
}

/**
 * @brief Destroy the Lock:: Lock object
 *
 */
Lock::~Lock()
{
#ifdef DEBUG
    log("Lock uninitialized");
#endif
}

/**
 * @brief Sets value to GPIO lock
 *
 * @param value Lock state
 * @return int
 */
int Lock::setLock(GPIO_VALUE value)
{
    return GPIO::setValue(value);
}
