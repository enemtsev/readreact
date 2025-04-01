#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/reactled_mock.h"
#include "readreact/reactclass.h"
#include "zephyr/drivers/gpio.h"

#include <zephyr/drivers/gpio/gpio_emul.h>

#define GPIO_EMUL_NODE DT_NODELABEL(gpio_emul)
#define EMUL_PIN_OUT 0

int on_count{0};
int off_count{0};

void gpio_callback_func(const struct device *dev_in, struct gpio_callback *gpio_cb, uint32_t pins) {
    int state = gpio_emul_output_get(dev_in, EMUL_PIN_OUT);
    if (state) {
        on_count++;
    } else {
        off_count++;
    }
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

    const struct device *gpio_dev = DEVICE_DT_GET(GPIO_EMUL_NODE);
    gpio_callback callback;
    gpio_pin_interrupt_configure(gpio_dev, EMUL_PIN_OUT, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&callback, gpio_callback_func, BIT(EMUL_PIN_OUT));
    gpio_add_callback(gpio_dev, &callback);

    react.handle_message({true});

    k_sleep(K_MSEC(1000));

    EXPECT_EQ(on_count, 3);
    EXPECT_EQ(off_count, 3);

    EXPECT_EQ(led.on_count, 3);
    EXPECT_EQ(led.off_count, 3);

    gpio_remove_callback(gpio_dev, &callback);
}

TEST(ReactClassTest, TurnsOnOnceOnLowGPIO) {
    on_count = 0;
    off_count = 0;

    MockReactLEDGPIO led;
    ReactClass react(&led);

    const struct device *gpio_dev = DEVICE_DT_GET(GPIO_EMUL_NODE);
    gpio_callback callback;
    gpio_pin_interrupt_configure(gpio_dev, EMUL_PIN_OUT, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&callback, gpio_callback_func, BIT(EMUL_PIN_OUT));
    gpio_add_callback(gpio_dev, &callback);

    react.handle_message({false});

    k_sleep(K_MSEC(1000));

    EXPECT_EQ(on_count, 1);
    EXPECT_EQ(off_count, 1);

    EXPECT_EQ(led.on_count, 1);
    EXPECT_EQ(led.off_count, 1);

    gpio_remove_callback(gpio_dev, &callback);
}