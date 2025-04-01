#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/gpio/gpio_emul.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "readreact/gpio_device.h"
#include "readreact/reactclass.h"
#include "readreact/reactled.h"
#include "readreact/readclass.h"
#include "readreact/zbusmanager.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

ZBusManager *zbusmanager_ptr;

// Define ZBus channel
ZBUS_CHAN_DEFINE(gpio_state_change_chan, struct ZBusMessage, NULL, &zbusmanager_ptr, ZBUS_OBSERVERS(), {});

int main(void) {
    ZBusManager manager;
    zbusmanager_ptr = &manager;
    manager.set_channel(&gpio_state_change_chan);

    ReactLED react_led;
    ReactClass reactor(&react_led);

    ReadClass reader;

    manager.register_publisher(&reader);
    manager.register_subscriber(&reactor);

    // emulate loop
    const device *gpio_dev = DEVICE_DT_GET(DEV_GPIO);
    int state{0};

    while (1) {
        k_sleep(K_MSEC(1000));

        if (state) {
            gpio_emul_input_set(gpio_dev, PIN_IN, 0);
            state = 0;
        } else {
            gpio_emul_input_set(gpio_dev, PIN_IN, 1);
            state = 1;
        }
    }

    return 0;
}