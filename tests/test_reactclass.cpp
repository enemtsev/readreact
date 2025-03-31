#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/reactled_mock.h"
#include "readreact/reactclass.h"

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