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
    ReadClass();
    ~ReadClass();

    bool init() override;

    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
private:
    struct CallbackContext {
        struct gpio_callback callback_;
        ReadClass *read_;
    };

    struct WorkContext {
        struct k_work_delayable work_;
        ReadClass *read_;
    };

    static void interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins);
    static void work_callback(struct k_work *work);

    void process_gpio_change();

    CallbackContext callback_context_;
    WorkContext work_context_;
    int last_state_ = -1;
    int64_t last_change_time_ = 0;

protected:
    const device *gpio_dev_{nullptr};
    gpio_pin_t pin_{0};
    gpio_flags_t flags_;

    bool init_gpio_device();
};