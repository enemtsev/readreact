#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gpio_mock.h"
#include "../include/readreact/gpio_observer.hpp"

extern GpioMock* gpioMock;

class ReadClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        gpioMock = new GpioMock();
    }

    void TearDown() override {
        delete gpioMock;
        gpioMock = nullptr;
    }
};

TEST_F(ReadClassTest, Initialization) {
    const device dummy_dev = {0};
    
    EXPECT_CALL(*gpioMock, pin_configure(&dummy_dev, 0, GPIO_INPUT | GPIO_PULL_UP))
        .WillOnce(Return(0));
    EXPECT_CALL(*gpioMock, pin_interrupt_configure(&dummy_dev, 0, GPIO_INT_EDGE_BOTH))
        .WillOnce(Return(0));
    EXPECT_CALL(*gpioMock, add_callback(&dummy_dev, _))
        .WillOnce(Return(0));

    ReadClass reader("input-gpio");
}

TEST_F(ReadClassTest, InterruptHandling) {
    const device dummy_dev = {0};
    gpio_callback saved_cb;
    
    // Setup expectations for initialization
    EXPECT_CALL(*gpioMock, pin_configure(&dummy_dev, 0, GPIO_INPUT | GPIO_PULL_UP))
        .WillOnce(Return(0));
    EXPECT_CALL(*gpioMock, pin_interrupt_configure(&dummy_dev, 0, GPIO_INT_EDGE_BOTH))
        .WillOnce(Return(0));
    EXPECT_CALL(*gpioMock, add_callback(&dummy_dev, _))
        .WillOnce(DoAll(SaveArg<1>(&saved_cb), Return(0)));

    ReadClass reader("input-gpio");

    // Simulate interrupt
    EXPECT_CALL(*gpioMock, pin_get(&dummy_dev, 0))
        .WillOnce(Return(1)); // High state

    // Trigger the callback
    saved_cb.handler(&dummy_dev, &saved_cb, BIT(0));

    // Verify zbus message was published
    // This would need additional mocking of zbus
}