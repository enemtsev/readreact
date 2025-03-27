#include "gpio_mock.h"

GpioMock* gpioMock = nullptr;

int gpio_pin_configure(const device *port, gpio_pin_t pin, gpio_flags_t flags) {
    return gpioMock->pin_configure(port, pin, flags);
}

int gpio_pin_interrupt_configure(const device *port, gpio_pin_t pin, gpio_flags_t flags) {
    return gpioMock->pin_interrupt_configure(port, pin, flags);
}

int gpio_pin_get(const device *port, gpio_pin_t pin) {
    return gpioMock->pin_get(port, pin);
}

int gpio_pin_set(const device *port, gpio_pin_t pin, int value) {
    return gpioMock->pin_set(port, pin, value);
}

int gpio_add_callback(const device *port, gpio_callback *callback) {
    return gpioMock->add_callback(port, callback);
}

int gpio_remove_callback(const device *port, gpio_callback *callback) {
    return gpioMock->remove_callback(port, callback);
}