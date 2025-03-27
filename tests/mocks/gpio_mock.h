#pragma once

#include <zephyr/drivers/gpio.h>
#include <gmock/gmock.h>

class GpioMock {
public:
    virtual ~GpioMock() = default;

    MOCK_METHOD(int, pin_configure, (const device *port, gpio_pin_t pin, gpio_flags_t flags));
    MOCK_METHOD(int, pin_interrupt_configure, (const device *port, gpio_pin_t pin, gpio_flags_t flags));
    MOCK_METHOD(int, pin_get, (const device *port, gpio_pin_t pin));
    MOCK_METHOD(int, pin_set, (const device *port, gpio_pin_t pin, int value));
    MOCK_METHOD(int, add_callback, (const device *port, gpio_callback *callback));
    MOCK_METHOD(int, remove_callback, (const device *port, gpio_callback *callback));
};

extern GpioMock* gpioMock;

// C interface wrapper
extern "C" {
    int gpio_pin_configure(const device *port, gpio_pin_t pin, gpio_flags_t flags);
    int gpio_pin_interrupt_configure(const device *port, gpio_pin_t pin, gpio_flags_t flags);
    int gpio_pin_get(const device *port, gpio_pin_t pin);
    int gpio_pin_set(const device *port, gpio_pin_t pin, int value);
    int gpio_add_callback(const device *port, gpio_callback *callback);
    int gpio_remove_callback(const device *port, gpio_callback *callback);
}