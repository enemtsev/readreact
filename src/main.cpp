#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "readreact/readclass.h"
#include "readreact/reactclass.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void)
{
    ReadClass reader("input-gpio");
    ReactClass reactor("output-gpio");

    while (1) {
        k_sleep(K_MSEC(1000));
    }

    return 0;
}