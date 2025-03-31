#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

/**
 * @class ReactClass
 * @brief Observes GPIO state changes and controls output GPIO accordingly
 */
 class ReactClass {
    public:
        /**
         * @brief Construct a new ReactClass object
         * @param output_pin Device tree label for output GPIO
         */
        ReactClass(const char* output_pin);
    
        ~ReactClass();
    
    private:
        void init();    
        static void observer_callback(const struct zbus_channel *chan);    
        void handle_gpio_change(bool state);    
        static void blink_work_callback(struct k_work *work);
    
        struct k_work_delayable blink_work_;
        const char* output_pin_;
        inline static bool last_state_ = false;
        uint8_t blink_count_ = 0;

        inline static ReactClass* instance_{nullptr};
    };