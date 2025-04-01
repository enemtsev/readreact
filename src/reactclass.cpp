#include "readreact/reactclass.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(react);

ReactClass::ReactClass(ReactLED *react_led)
    : react_led_(react_led) {
    init();  // Initialize the GPIO and work structures.
}

// Initialize the GPIO, work structures, and setup the observer
void ReactClass::init() {
    react_led_->init();
    work_context_.react_ = this;
    k_work_init_delayable(&work_context_.blink_work_, blink_work_callback);
    LOG_INF("ReactClass initialized for output");
}

// This method overrides the handle_message() from BaseSubscriber
void ReactClass::handle_message(const struct ZBusMessage &msg) {
    last_state_ = msg.state;
    k_work_schedule(&work_context_.blink_work_, K_NO_WAIT);
}

// This function is called by the work manager for blinking the LED.
void ReactClass::blink_work_callback(struct k_work *work) {
    k_work_delayable *dwork = k_work_delayable_from_work(work);
    WorkContext *ctx = CONTAINER_OF(dwork, WorkContext, blink_work_);

    if (ctx->react_->last_state_) {  // If state is HIGH
        // Blink the LED 3 times with 100ms delay
        for (int i = 0; i < 3; ++i) {
            ctx->react_->react_led_->turn_on();
            k_sleep(K_MSEC(100));
            ctx->react_->react_led_->turn_off();
            k_sleep(K_MSEC(100));
        }
    } else {  // If state is LOW
        // Keep the LED on for 500ms
        ctx->react_->react_led_->turn_on();
        k_sleep(K_MSEC(500));
        ctx->react_->react_led_->turn_off();
    }
}
