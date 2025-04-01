#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/zbus_mocks.h"
#include "readreact/zbusmanager.h"  // Assuming the class is in this header file

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

ZBusManager *testmanager_ptr{nullptr};

// Define ZBus channel
ZBUS_CHAN_DEFINE(test_chan, struct ZBusMessage, NULL, &testmanager_ptr, ZBUS_OBSERVERS(), {});

// Test class for ZBusManager
class ZBusManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        testmanager_ptr = new ZBusManager();
        testmanager_ptr->set_channel(&test_chan);
    }

    void TearDown() override {
        delete testmanager_ptr;
    }
};

// Test: Verify that the subscriber is triggered when a message is published
TEST_F(ZBusManagerTest, TestPublishMessage) {
    MockPublisher publisher_;
    EXPECT_CALL(publisher_, init()).Times(1);
    testmanager_ptr->register_publisher(&publisher_);

    MockSubscriber subscriber;
    testmanager_ptr->register_subscriber(&subscriber);

    // Prepare the message
    ZBusMessage test_message{true};  // State set to HIGH

    // Expect the subscriber's handle_message method to be called once with the
    // test message
    EXPECT_CALL(subscriber, handle_message(test_message)).Times(1);

    // Simulate publishing a message
    publisher_.publish(test_message);
}

// Test: Verify that multiple subscribers receive the message
TEST_F(ZBusManagerTest, TestMultipleSubscribers) {
    MockPublisher publisher_;
    EXPECT_CALL(publisher_, init()).Times(1);
    testmanager_ptr->register_publisher(&publisher_);

    MockSubscriber subscriber;
    testmanager_ptr->register_subscriber(&subscriber);

    // Create another mock subscriber
    MockSubscriber subscriber2;
    testmanager_ptr->register_subscriber(&subscriber2);

    // Prepare the message
    ZBusMessage test_message{false};  // State set to LOW

    // Expect the first subscriber's handle_message method to be called once
    EXPECT_CALL(subscriber, handle_message(test_message)).Times(1);

    // Expect the second subscriber's handle_message method to be called once
    EXPECT_CALL(subscriber2, handle_message(test_message)).Times(1);

    // Simulate publishing a message
    publisher_.publish(test_message);
}

// Test: Verify that no crash when no subscribers are set
TEST_F(ZBusManagerTest, TestNoPublishCallback) {
    MockPublisher publisher_;
    EXPECT_CALL(publisher_, init()).Times(1);
    testmanager_ptr->register_publisher(&publisher_);

    // Prepare a message
    ZBusMessage test_message{true};  // State set to HIGH

    // Try publishing without setting a publish callback
    EXPECT_NO_FATAL_FAILURE(publisher_.publish(test_message));
}