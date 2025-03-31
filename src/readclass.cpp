#include "readreact/readclass.h"
#include "readreact/gpio_observer.hpp"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(read);

ReadClass::ReadClass(const char *input_pin)
    : input_pin_(input_pin) {
    init();
}

void ReadClass::init() {
    // Get GPIO device using device-tree compatible
    const device *gpio_dev = device_get_binding("GPIO_0");
    if (!device_is_ready(gpio_dev)) {
        LOG_ERR("Input GPIO device not ready");
        return;
    }

    // Configure input pin
    int ret = gpio_pin_configure(gpio_dev, 0, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        LOG_ERR("Failed to configure input GPIO: %d", ret);
        return;
    }

    // Configure interrupt
    ret = gpio_pin_interrupt_configure(gpio_dev, 0, GPIO_INT_EDGE_BOTH);
    if (ret < 0) {
        LOG_ERR("Failed to configure interrupt: %d", ret);
        return;
    }

    // Initialize work queue
    k_work_init(&work_, work_callback);

    // Setup interrupt callback
    gpio_init_callback(&callback_, interrupt_callback, BIT(0));
    ret = gpio_add_callback(gpio_dev, &callback_);
    if (ret < 0) {
        LOG_ERR("Failed to add callback: %d", ret);
        return;
    }

    LOG_INF("ReadClass initialized for pin %s", input_pin_);
}

void ReadClass::interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins) {
    ReadClass *self = CONTAINER_OF(cb, ReadClass, callback_);
    k_work_submit(&self->work_);
}

void ReadClass::work_callback(struct k_work *work) {
    ReadClass *self = CONTAINER_OF(work, ReadClass, work_);
    self->process_gpio_change();
}

void ReadClass::process_gpio_change() {
    const device *gpio_dev = device_get_binding("GPIO_0");
    int state = gpio_pin_get(gpio_dev, 0);

    // Debounce logic
    if (k_uptime_get() - last_change_time_ < DEBOUNCE_TIME_MS) {
        return;
    }

    if (state != last_state_) {
        last_state_ = state;
        last_change_time_ = k_uptime_get();

        struct gpio_state_change msg = {.state = (bool)state};
        zbus_chan_pub(&gpio_state_change_chan, &msg, K_NO_WAIT);
        LOG_INF("GPIO state changed to %s", state ? "HIGH" : "LOW");
    }
}