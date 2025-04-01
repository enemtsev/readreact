#pragma once

#include "readreact/readclass.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define GPIO_EMUL_NODE DT_NODELABEL(gpio_emul)
#define EMUL_PIN_IN 1

class MockReadGPIO : public ReadClass {
public:
    bool init() override {
        gpio_dev_ = DEVICE_DT_GET(GPIO_EMUL_NODE);
        pin_ = EMUL_PIN_IN;
        flags_ = GPIO_INPUT; 

        return init_gpio_device();
    }
};