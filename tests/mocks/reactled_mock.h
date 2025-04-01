#pragma once

#include "readreact/reactled.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


class MockReactLED : public ReactLED {
public:
    MOCK_METHOD(void, turn_on, (), (override));
    MOCK_METHOD(void, turn_off, (), (override));

    bool init() override {
        return true;
    }
};

class MockReactLEDGPIO : public ReactLED {
public:
    int on_count{0};
    int off_count{0};

    bool init() override;
    void turn_on() override;
    void turn_off() override;
};