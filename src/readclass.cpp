#include "readreact/readclass.h"
#include "readreact/gpio_device.h"

#include <zephyr/logging/log.h>
#include <zephyr/sys_clock.h>
#include <gmock/gmock.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(read, LOG_LEVEL_DBG);

ReadClass::ReadClass() {
    callback_context_.read_ = this;
    work_context_.read_ = this;

    k_work_init_delayable(&work_context_.work_, work_callback);
}

ReadClass::~ReadClass() {
    k_work_cancel_delayable(&work_context_.work_);
    gpio_remove_callback(gpio_dev_, &callback_context_.callback_);
}

bool ReadClass::init_gpio_device() {
    if (!device_is_ready(gpio_dev_)) {
        LOG_ERR("Input GPIO device not ready");
        return false;
    }

    int ret = gpio_pin_configure(gpio_dev_, pin_, flags_);
    if (ret < 0) {
        LOG_ERR("Failed to configure input GPIO: %d", ret);
        return false;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev_, pin_, GPIO_INT_EDGE_BOTH);
    if (ret < 0) {
        LOG_ERR("Failed to configure interrupt: %d", ret);
        return false;
    }

    gpio_init_callback(&callback_context_.callback_, interrupt_callback, BIT(pin_));

    ret = gpio_add_callback(gpio_dev_, &callback_context_.callback_);
    if (ret < 0) {
        LOG_ERR("Failed to add callback: %d", ret);
        return false;
    }

    return true;
}

bool ReadClass::init() {
    gpio_dev_ = DEVICE_DT_GET(DEV_GPIO);
    pin_ = PIN_IN;
    flags_ = DEV_IN_CONFIG;

    bool ret = init_gpio_device();

    LOG_INF("ReadClass initialized for input pin %d", ret);
    
    return ret;
}

void ReadClass::interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins) {
    CallbackContext *ctx = CONTAINER_OF(cb, CallbackContext, callback_);
    k_work_schedule(&ctx->read_->work_context_.work_, K_NO_WAIT);
}

void ReadClass::work_callback(struct k_work *work) {
    k_work_delayable *dwork = k_work_delayable_from_work(work);
    WorkContext *ctx = CONTAINER_OF(dwork, WorkContext, work_);
    ctx->read_->process_gpio_change();
}

void ReadClass::process_gpio_change() {
    int state = gpio_pin_get(gpio_dev_, pin_);
    int64_t time = k_uptime_get() - last_change_time_;
    LOG_DBG("process_gpio_change state: %d %d", state, time);
    
    if (time > DEBOUNCE_TIME_MS && state != last_state_) {
        last_state_ = state;
        last_change_time_ = k_uptime_get();

        ZBusMessage msg = {static_cast<bool>(state)};
        publish(msg);  // Use BasePublisher's publish function

        LOG_INF("GPIO state changed to %s", state ? "HIGH" : "LOW");
    }
}
