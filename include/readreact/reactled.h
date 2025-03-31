#pragma once

#include <zephyr/drivers/gpio.h>

/**
 * @class ReactLED
 * @brief Base class for controlling an ReactLED.
 */
 class ReactLED {
public:
    ReactLED();
    virtual ~ReactLED() = default;
    
    virtual void turn_on();
    virtual void turn_off();

private:
    const struct device *gpio_dev;
};
