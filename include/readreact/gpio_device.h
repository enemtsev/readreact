#pragma once

#include <zephyr/drivers/gpio.h>

#define DEV_GPIO DT_NODELABEL(gpio_emul)
#define DEV_OUT_CONFIG GPIO_OUTPUT_ACTIVE
#define DEV_IN_CONFIG GPIO_INPUT

#define PIN_OUT 0
#define PIN_IN 1

// some real device
// #define DEV_OUT DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), out_gpios)
// #define PIN_OUT DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), out_gpios)
// #define PIN_OUT_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), out_gpios)

// #define DEV_IN DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), in_gpios)
// #define PIN_IN DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), in_gpios)
// #define PIN_IN_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), in_gpios)