#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/reactled_mock.h"
#include "readreact/reactclass.h"
#include "zephyr/drivers/gpio.h"

#define DEV_OUT DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), out_gpios)
#define PIN_OUT_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), out_gpios)

int on_count{0};
int off_count{0};

void gpio_callback_func(const struct device *dev_in, struct gpio_callback *gpio_cb, uint32_t pins) {
    // if (pins & PIN_OUT) {
    int state = gpio_pin_get(dev_in, PIN_OUT);
    if (state) {
        on_count++;
    } else {
        off_count++;
    }
    // }
}

TEST(ReactClassTest, BlinksThreeTimesOnHigh) {
    MockReactLED led;
    ReactClass react(&led);
    EXPECT_CALL(led, turn_on()).Times(3);
    EXPECT_CALL(led, turn_off()).Times(3);
    react.handle_message({true});

    k_sleep(K_MSEC(1000));
}

TEST(ReactClassTest, TurnsOnOnceOnLow) {
    MockReactLED led;
    ReactClass react(&led);
    EXPECT_CALL(led, turn_on()).Times(1);
    EXPECT_CALL(led, turn_off()).Times(1);
    react.handle_message({false});

    k_sleep(K_MSEC(1000));
}

TEST(ReactClassTest, BlinksThreeTimesOnHighGPIO) {
    on_count = 0;
    off_count = 0;

    MockReactLEDGPIO led;
    ReactClass react(&led);

    const struct device *gpio_dev = DEVICE_DT_GET(DEV_OUT);
    gpio_callback callback;
    gpio_pin_interrupt_configure(gpio_dev, PIN_OUT, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&callback, gpio_callback_func, BIT(PIN_OUT));
    gpio_add_callback(gpio_dev, &callback);

    react.handle_message({true});

    k_sleep(K_MSEC(1000));

    // EXPECT_EQ(on_count, 3);
    EXPECT_EQ(off_count, 6);

    EXPECT_EQ(led.on_count, 3);
    EXPECT_EQ(led.off_count, 3);

    gpio_remove_callback(gpio_dev, &callback);
}

TEST(ReactClassTest, TurnsOnOnceOnLowGPIO) {
    on_count = 0;
    off_count = 0;

    MockReactLEDGPIO led;
    ReactClass react(&led);

    const struct device *gpio_dev = DEVICE_DT_GET(DEV_OUT);
    gpio_callback callback;
    gpio_pin_interrupt_configure(gpio_dev, PIN_OUT, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&callback, gpio_callback_func, BIT(PIN_OUT));
    gpio_add_callback(gpio_dev, &callback);

    react.handle_message({false});

    k_sleep(K_MSEC(1000));

    // EXPECT_EQ(on_count, 1);
    EXPECT_EQ(off_count, 2);

    EXPECT_EQ(led.on_count, 1);
    EXPECT_EQ(led.off_count, 1);

    gpio_remove_callback(gpio_dev, &callback);
}