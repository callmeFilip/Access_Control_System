#ifndef LOCK_H
#define LOCK_H

#include "GPIO.hpp"
#include "IOManager.hpp"

class Lock : public GPIO
{
public:
    Lock(int gpio_number, IOManager &IO_manager);
    ~Lock();
    int setLock(GPIO_VALUE value);
};

#endif // Lock
