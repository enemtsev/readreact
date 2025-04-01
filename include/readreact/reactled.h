#pragma once

#include <zephyr/drivers/gpio.h>

/**
 * @class ReactLED
 * @brief Base class for controlling an ReactLED.
 */
class ReactLED {
public:
    ReactLED() = default;
    virtual ~ReactLED() = default;

    virtual bool init();
    virtual void turn_on();
    virtual void turn_off();

protected:
    const struct device *gpio_dev_;
    int pin_{0};
};
