#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "readreact/reactclass.h"
#include "readreact/reactled.h"
#include "readreact/readclass.h"
#include "readreact/zbusmanager.h"
#include "zephyr/drivers/gpio.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define DEV_IN DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), in_gpios)

ZBusManager *zbusmanager_ptr;

// Define ZBus channel
ZBUS_CHAN_DEFINE(gpio_state_change_chan, struct ZBusMessage, NULL, &zbusmanager_ptr, ZBUS_OBSERVERS(), {});

int main(void) {
    ZBusManager manager;
    zbusmanager_ptr = &manager;

    // ReadClass reader("input-gpio");
    ReactLED react_led;
    ReactClass reactor(&react_led);

    ReadClass reader;

    manager.register_publisher(&reader);
    manager.register_subscriber(&reactor);

    while (1) {
        k_sleep(K_MSEC(1000));
        const device *gpio_dev = DEVICE_DT_GET(DEV_IN);

        gpio_pin_toggle(gpio_dev, PIN_IN);
    }

    return 0;
}