#include "reactled_mock.h"

#include <zephyr/logging/log.h>

#define GPIO_EMUL_NODE DT_NODELABEL(gpio_emul)
#define EMUL_PIN_OUT 0

LOG_MODULE_REGISTER(reactled_mock);

bool MockReactLEDGPIO::init() {
    pin_ = EMUL_PIN_OUT;
    gpio_dev_ = DEVICE_DT_GET(GPIO_EMUL_NODE);
    if (!device_is_ready(gpio_dev_)) {
        LOG_ERR("Error: GPIO emulator device not ready\n");
        return false;
    }

    int ret = gpio_pin_configure(gpio_dev_, pin_, GPIO_OUTPUT_ACTIVE);

    if (ret < 0) {
        LOG_ERR("Failed to configure GPIO pin %d\n", pin_);
        return false;
    }
    return true;
}

void MockReactLEDGPIO::turn_on() {
    on_count++;
    ReactLED::turn_on();
}

void MockReactLEDGPIO::turn_off() {
    off_count++;
    ReactLED::turn_off();
}
