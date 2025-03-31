#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

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

    // ReadClass reader("input-gpio");
    ReactLED react_led;
    ReactClass reactor(&react_led);

    manager.register_subscriber(&reactor);

    while (1) {
        k_sleep(K_MSEC(1000));
    }

    return 0;
}