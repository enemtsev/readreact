#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/gpio_mock.h"

#include "readreact/gpio_observer.hpp"

extern GpioMock* gpioMock;

class ReactClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        gpioMock = new GpioMock();
    }

    void TearDown() override {
        delete gpioMock;
        gpioMock = nullptr;
    }
};

TEST_F(ReactClassTest, Initialization) {
    const device dummy_dev = {0};
    
    EXPECT_CALL(*gpioMock, pin_configure(&dummy_dev, 1, GPIO_OUTPUT))
        .WillOnce(Return(0));

    ReactClass reactor("output-gpio");
}

TEST_F(ReactClassTest, HighStateHandling) {
    const device dummy_dev = {0};
    
    // Setup expectations for initialization
    EXPECT_CALL(*gpioMock, pin_configure(&dummy_dev, 1, GPIO_OUTPUT))
        .WillOnce(Return(0));

    ReactClass reactor("output-gpio");

    // Expect 3 blinks (6 state changes)
    EXPECT_CALL(*gpioMock, pin_set(&dummy_dev, 1, 1)).Times(3);
    EXPECT_CALL(*gpioMock, pin_set(&dummy_dev, 1, 0)).Times(3);

    // Simulate high state change
    gpio_state_change msg = {.state = true};
    zbus_chan_pub(&gpio_state_change_chan, &msg, K_NO_WAIT);

    // Need to process work queue - in real test would advance time
}

TEST_F(ReactClassTest, LowStateHandling) {
    const device dummy_dev = {0};
    
    // Setup expectations for initialization
    EXPECT_CALL(*gpioMock, pin_configure(&dummy_dev, 1, GPIO_OUTPUT))
        .WillOnce(Return(0));

    ReactClass reactor("output-gpio");

    // Expect LED to turn on then off
    EXPECT_CALL(*gpioMock, pin_set(&dummy_dev, 1, 1)).Times(1);
    EXPECT_CALL(*gpioMock, pin_set(&dummy_dev, 1, 0)).Times(1);

    // Simulate low state change
    gpio_state_change msg = {.state = false};
    zbus_chan_pub(&gpio_state_change_chan, &msg, K_NO_WAIT);

    // Need to process work queue - in real test would advance time
}