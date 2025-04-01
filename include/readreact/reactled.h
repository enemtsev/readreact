/**
 * @file reactled.h
 * @brief Definition of the ReactLED class for LED control abstraction
 */

#pragma once

#include <zephyr/drivers/gpio.h>

/**
 * @class ReactLED
 * @brief Base class for controlling an LED via GPIO.
 *
 * This class provides a hardware abstraction layer for LED control.
 * It encapsulates the GPIO device access and pin configuration,
 * providing a simple interface for turning the LED on and off.
 *
 * The abstraction allows for easy mocking during unit tests.
 */
class ReactLED {
public:
    /**
     * @brief Default constructor
     */
    ReactLED() = default;

    /**
     * @brief Virtual destructor to enable proper cleanup in derived classes
     */
    virtual ~ReactLED() = default;

    /**
     * @brief Initialize the LED hardware
     *
     * Configures the GPIO pin for output and ensures the device is ready
     *
     * @return true if initialization successful
     * @return false if initialization failed
     */
    virtual bool init();

    /**
     * @brief Turn the LED on
     *
     * Sets the GPIO pin to high state
     */
    virtual void turn_on();

    /**
     * @brief Turn the LED off
     *
     * Sets the GPIO pin to low state
     */
    virtual void turn_off();

protected:
    /** @brief Pointer to the GPIO device */
    const struct device *gpio_dev_;

    /** @brief GPIO pin number */
    int pin_{0};
};