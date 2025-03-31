#include "readreact/readclass.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(read, LOG_LEVEL_DBG);

#define DEV_IN DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), in_gpios)

ReadClass::ReadClass() {
    init();
}

void ReadClass::init() {
    callback_context_.read_ = this;
    work_context_.read_ = this;

    const device *gpio_dev = DEVICE_DT_GET(DEV_IN);

    if (!device_is_ready(gpio_dev)) {
        LOG_ERR("Input GPIO device not ready");
        return;
    }

    int ret = gpio_pin_configure(gpio_dev, PIN_IN, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        LOG_ERR("Failed to configure input GPIO: %d", ret);
        return;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev, PIN_IN, GPIO_INT_EDGE_BOTH);
    if (ret < 0) {
        LOG_ERR("Failed to configure interrupt: %d", ret);
        return;
    }

    k_work_init(&work_context_.work_, work_callback);
    gpio_init_callback(&callback_context_.callback_, interrupt_callback, BIT(PIN_IN));

    ret = gpio_add_callback(gpio_dev, &callback_context_.callback_);
    if (ret < 0) {
        LOG_ERR("Failed to add callback: %d", ret);
        return;
    }

    LOG_INF("ReadClass initialized for input pin");
}

void ReadClass::interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins) {
    CallbackContext *ctx = CONTAINER_OF(cb, CallbackContext, callback_);
    ctx->read_->work_context_.dev = dev;
    k_work_submit(&ctx->read_->work_context_.work_);
}

void ReadClass::work_callback(struct k_work *work) {
    WorkContext *ctx = CONTAINER_OF(work, WorkContext, work_);
    ctx->read_->process_gpio_change(ctx->dev);
}

void ReadClass::process_gpio_change(const device *dev) {
    int state = gpio_pin_get(dev, PIN_IN);

    if (k_uptime_get() - last_change_time_ < DEBOUNCE_TIME_MS) {
        return;
    }

    if (state != last_state_) {
        last_state_ = state;
        last_change_time_ = k_uptime_get();

        ZBusMessage msg = {static_cast<bool>(state)};
        // publish(msg);  // Use BasePublisher's publish function

        LOG_INF("GPIO state changed to %s", state ? "HIGH" : "LOW");
    }
}
