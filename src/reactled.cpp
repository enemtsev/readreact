#include "readreact/reactled.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(reactled);

#define DEV_OUT DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), out_gpios)

ReactLED::ReactLED()
    : gpio_dev{DEVICE_DT_GET(DEV_OUT)} {
    if (!device_is_ready(gpio_dev)) {
        LOG_ERR("Output GPIO device not ready");
        return;
    }

    int ret = gpio_pin_configure(gpio_dev, PIN_OUT, GPIO_OUTPUT_LOW | PIN_OUT_FLAGS);
    if (ret < 0) {
        LOG_ERR("Failed to configure output GPIO: %d", ret);
        return;
    }
}

void ReactLED::turn_on() {
    LOG_INF("LED on");
    if (gpio_dev != nullptr) {
        gpio_pin_set(gpio_dev, PIN_OUT, 1);
    } else {
        LOG_ERR("LED device error");
    }
}

void ReactLED::turn_off() {
    LOG_INF("LED off");
    if (gpio_dev != nullptr) {
        gpio_pin_set(gpio_dev, PIN_OUT, 0);
    } else {
        LOG_ERR("LED device error");
    }
}