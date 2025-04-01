#include "readreact/reactled.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(reactled);

#define DEV_OUT DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), out_gpios)

bool ReactLED::init() {
    pin_ = PIN_OUT;
    gpio_dev_ = DEVICE_DT_GET(DEV_OUT);
    if (!device_is_ready(gpio_dev_)) {
        LOG_ERR("Output GPIO device not ready");
        return false;
    }

    int ret = gpio_pin_configure(gpio_dev_, pin_, PIN_OUT_FLAGS);
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