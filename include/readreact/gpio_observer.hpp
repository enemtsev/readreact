#pragma once

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief GPIO state change message structure
 */
struct gpio_state_change {
    bool state;
};

// ZBus channel declaration
ZBUS_CHAN_DECLARE(gpio_state_change_chan);