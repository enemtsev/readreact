#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/read_mock.h"
#include "readreact/readclass.h"

#include <zephyr/drivers/gpio/gpio_emul.h>

class ReadClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        gpio_dev = DEVICE_DT_GET(GPIO_EMUL_NODE);
        ASSERT_TRUE(device_is_ready(gpio_dev));
    }

    void TearDown() override {
        gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 0);
    }

    const struct device *gpio_dev;
};

TEST_F(ReadClassTest, DetectsHighSignal) {
    int callback_count{0};

    MockReadGPIO read;
    read.init();
    read.set_publish_callback([&callback_count](const ZBusMessage &) {
        ++callback_count;
    });

    // Simulate a HIGH signal on the GPIO pin
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);

    // Wait for debounce period plus a little extra
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    EXPECT_EQ(callback_count, 1);
}

TEST_F(ReadClassTest, DetectsLowSignal) {
    int callback_count{0};
    MockReadGPIO read;
    read.init();
    read.set_publish_callback([&callback_count](const ZBusMessage &) {
        ++callback_count;
    });

    // First set to HIGH so we can detect a LOW transition
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    EXPECT_EQ(callback_count, 1);

    callback_count = 0;

    // Now simulate a LOW signal
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 0);
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    EXPECT_EQ(callback_count, 1);
}

TEST_F(ReadClassTest, DebouncesProperly) {
    int callback_count{0};
    MockReadGPIO read;
    read.init();
    read.set_publish_callback([&callback_count](const ZBusMessage &) {
        ++callback_count;
    });

    // Simulate multiple HIGH signals in quick succession (within debounce time)
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);
    k_sleep(K_MSEC(10));  // Less than debounce time
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 0);
    k_sleep(K_MSEC(10));  // Less than debounce time
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);

    // Wait for debounce period plus a little extra
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    EXPECT_EQ(callback_count, 1);
}

TEST_F(ReadClassTest, HandlesMultipleStateChanges) {
    int callback_count{0};
    MockReadGPIO read;
    read.init();
    read.set_publish_callback([&callback_count](const ZBusMessage &) {
        ++callback_count;
    });

    // First transition: LOW to HIGH
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    // Second transition: HIGH to LOW
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 0);
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    // Third transition: LOW to HIGH
    gpio_emul_input_set(gpio_dev, EMUL_PIN_IN, 1);
    k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

    EXPECT_EQ(callback_count, 3);
}