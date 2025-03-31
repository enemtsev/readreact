#pragma once

#include "readreact/reactled.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockReactLED : public ReactLED {
public:
    MOCK_METHOD(void, turn_on, (), (override));
    MOCK_METHOD(void, turn_off, (), (override));
};

class MockReactLEDGPIO : public ReactLED {
    public:
        int on_count{0};
        int off_count{0};


        void turn_on() override {
            on_count++;
            ReactLED::turn_on();
        }

        void turn_off() override {
            off_count++;
            ReactLED::turn_off();
        }
};