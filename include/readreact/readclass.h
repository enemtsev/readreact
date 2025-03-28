#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

/**
 * @class ReadClass
 * @brief Handles GPIO input with debouncing and publishes state changes via ZBus
 */
 class ReadClass {
    public:
        /**
         * @brief Construct a new ReadClass object
         * @param input_pin Device tree label for input GPIO
         */
        ReadClass(const char* input_pin);
    
    private:
        void init();
    
        static void interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins);
        static void work_callback(struct k_work *work);
            
        void process_gpio_change();
    
        static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
        const char* input_pin_;
        gpio_callback callback_;
        struct k_work work_;
        int last_state_ = -1;
        int64_t last_change_time_ = 0;
    };
    