#pragma once

#include "readreact/reactled.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockReactLED : public ReactLED {
public:
    MOCK_METHOD(void, turn_on, (), (override));
    MOCK_METHOD(void, turn_off, (), (override));
};