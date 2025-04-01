
/**
 * @file gpio_device.h
 * @brief GPIO device configuration defines for the ReadReact project
 *
 * This file contains the definitions for GPIO devices, pins and configurations
 * used throughout the ReadReact project. It centralizes hardware-specific definitions
 * to facilitate testing and portability.
 */

#pragma once

#include <zephyr/drivers/gpio.h>

/** @brief GPIO device node label for emulation */
#define DEV_GPIO DT_NODELABEL(gpio_emul)

/** @brief Configuration for output pins - active high */
#define DEV_OUT_CONFIG GPIO_OUTPUT_ACTIVE

/** @brief Configuration for input pins */
#define DEV_IN_CONFIG GPIO_INPUT

/** @brief Pin number for the output (LED) */
#define PIN_OUT 0

/** @brief Pin number for the input (button/sensor) */
#define PIN_IN 1

/**
 * @note The following definitions are commented out but would be used
 * for real hardware instead of emulation
 */
// #define DEV_OUT DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), out_gpios)
// #define PIN_OUT DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), out_gpios)
// #define PIN_OUT_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), out_gpios)

// #define DEV_IN DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), in_gpios)
// #define PIN_IN DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), in_gpios)
// #define PIN_IN_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), in_gpios)