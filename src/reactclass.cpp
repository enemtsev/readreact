#include "readreact/reactclass.h"
#include "readreact/gpio_observer.hpp"

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(react);

// Define ZBus channel
ZBUS_CHAN_DEFINE(gpio_state_change_chan,
    struct gpio_state_change,
    NULL,
    NULL,
    ZBUS_OBSERVERS(),
    {}
);

ReactClass::ReactClass(const char* output_pin) : output_pin_(output_pin) {
    instance_ = this;
    init();

    // ZBus observer setup
    // Initialize observer structure
    observer_.callback = observer_callback;

    zbus_chan_add_obs(&gpio_state_change_chan, &observer_, K_NO_WAIT);
}

ReactClass::~ReactClass() {
    zbus_chan_rm_obs(&gpio_state_change_chan, &observer_, K_NO_WAIT);
}

void ReactClass::init() {
    const device *gpio_dev = device_get_binding("GPIO_1");
    if (!device_is_ready(gpio_dev)) {
        LOG_ERR("Output GPIO device not ready");
        return;
    }

    int ret = gpio_pin_configure(gpio_dev, 0, GPIO_OUTPUT);
    if (ret < 0) {
        LOG_ERR("Failed to configure output GPIO: %d", ret);
        return;
    }

    k_work_init_delayable(&blink_work_, blink_work_callback);
    LOG_INF("ReactClass initialized for pin %s", output_pin_);
}

void ReactClass::observer_callback(const struct zbus_channel *chan) {
    struct gpio_state_change msg;
    zbus_chan_read(chan, &msg, K_NO_WAIT);
    instance_->handle_gpio_change(msg.state);
}

void ReactClass::handle_gpio_change(bool state) {
    const device *gpio_dev = device_get_binding("GPIO_1");
    
    if (state) {
        // High state - blink 3 times
        blink_count_ = 0;
        k_work_schedule(&blink_work_, K_MSEC(100));
    } else {
        // Low state - turn on for 500ms
        gpio_pin_set(gpio_dev, 0, 1);
        k_work_schedule(&blink_work_, K_MSEC(500));
    }
}

void ReactClass::blink_work_callback(struct k_work *work) {
    ReactClass* instance = CONTAINER_OF(work, ReactClass, blink_work_);
    const device *gpio_dev = device_get_binding("GPIO_1");

    if (instance->last_state_) {
        // Handle blinking for high state
        instance->blink_count_++;
        if (instance->blink_count_ <= 6) { // 3 on/off cycles
            bool led_state = (instance->blink_count_ % 2) == 1;
            gpio_pin_set(gpio_dev, 0, led_state);
            if (instance->blink_count_ < 6) {
                k_work_schedule(&instance->blink_work_, K_MSEC(100));
            }
        }
    } else {
        // Handle 500ms on for low state
        gpio_pin_set(gpio_dev, 0, 0);
    }
}
