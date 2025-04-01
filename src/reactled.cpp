#include "readreact/reactled.h"
#include "readreact/gpio_device.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(reactled);

bool ReactLED::init() {
    pin_ = PIN_OUT;
    gpio_dev_ = DEVICE_DT_GET(DEV_GPIO);
    if (!device_is_ready(gpio_dev_)) {
        LOG_ERR("Output GPIO device not ready");
        return false;
    }

    int ret = gpio_pin_configure(gpio_dev_, pin_, DEV_OUT_CONFIG);
    if (ret < 0) {
        LOG_ERR("Failed to configure output GPIO: %d", ret);
        return false;
    }

    return true;
}

void ReactLED::turn_on() {
    LOG_INF("LED on");
    if (gpio_dev_ != nullptr) {
        gpio_pin_set(gpio_dev_, pin_, 1);
    } else {
        LOG_ERR("LED device error");
    }
}

void ReactLED::turn_off() {
    LOG_INF("LED off");
    if (gpio_dev_ != nullptr) {
        gpio_pin_set(gpio_dev_, pin_, 0);
    } else {
        LOG_ERR("LED device error");
    }
}