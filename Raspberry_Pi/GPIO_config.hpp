#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#define GPIO_PATH "/sys/class/gpio"
#define GPIO_EXPORT_PATH GPIO_PATH "/export"
#define GPIO_UNEXPORT_PATH GPIO_PATH "/unexport"
#define GPIO_NUMBER_PATH(number) GPIO_PATH "/gpio" + std::to_string(number)
#define GPIO_DIRECTION_PATH(number) GPIO_NUMBER_PATH(number) + "/direction"
#define GPIO_VALUE_PATH(number) GPIO_NUMBER_PATH(number) + "/value"

#endif // GPIO_CONFIG_H