#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "readreact/zbusmanager.h"

/**
 * @class ReadClass
 * @brief Handles GPIO input with debouncing and publishes state changes via ZBus
 */
class ReadClass : public BasePublisher {
public:
    /**
     * @brief Construct a new ReadClass object
     */
    explicit ReadClass();

    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;

private:
    struct CallbackContext {
        struct gpio_callback callback_;
        ReadClass *read_;
    };

    struct WorkContext {
        struct k_work work_;
        ReadClass *read_;
        const device *dev;
    };

    void init();

    static void interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins);
    static void work_callback(struct k_work *work);

    void process_gpio_change(const device *dev);

    CallbackContext callback_context_;
    WorkContext work_context_;
    int last_state_ = -1;
    int64_t last_change_time_ = 0;
};