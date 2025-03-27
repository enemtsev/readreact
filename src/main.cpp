#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/logging/log.h>
#include "gpio_observer.hpp"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

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
    ReadClass(const char* input_pin) : input_pin_(input_pin) {
        init();
    }

private:
    void init() {
        // Get GPIO device
        const device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(input_gpio));
        if (!device_is_ready(gpio_dev)) {
            LOG_ERR("Input GPIO device not ready");
            return;
        }

        // Configure input pin
        int ret = gpio_pin_configure(gpio_dev, get_pin_number(input_pin_), 
                    GPIO_INPUT | GPIO_PULL_UP);
        if (ret < 0) {
            LOG_ERR("Failed to configure input GPIO: %d", ret);
            return;
        }

        // Configure interrupt
        ret = gpio_pin_interrupt_configure(gpio_dev, get_pin_number(input_pin_),
                    GPIO_INT_EDGE_BOTH);
        if (ret < 0) {
            LOG_ERR("Failed to configure interrupt: %d", ret);
            return;
        }

        // Initialize work queue
        k_work_init(&work_, work_callback);

        // Setup interrupt callback
        gpio_init_callback(&callback_, interrupt_callback, BIT(get_pin_number(input_pin_)));
        ret = gpio_add_callback(gpio_dev, &callback_);
        if (ret < 0) {
            LOG_ERR("Failed to add callback: %d", ret);
            return;
        }

        LOG_INF("ReadClass initialized for pin %s", input_pin_);
    }

    static void interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins) {
        ReadClass *self = CONTAINER_OF(cb, ReadClass, callback_);
        k_work_submit(&self->work_);
    }

    static void work_callback(struct k_work *work) {
        ReadClass *self = CONTAINER_OF(work, ReadClass, work_);
        self->process_gpio_change();
    }

    void process_gpio_change() {
        const device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(input_gpio));
        int state = gpio_pin_get(gpio_dev, get_pin_number(input_pin_));

        // Debounce logic
        if (k_uptime_get() - last_change_time_ < DEBOUNCE_TIME_MS) {
            return;
        }

        if (state != last_state_) {
            last_state_ = state;
            last_change_time_ = k_uptime_get();

            gpio_state_change msg = {.state = (bool)state};
            zbus_chan_pub(&gpio_state_change_chan, &msg, K_NO_WAIT);
            LOG_INF("GPIO state changed to %s", state ? "HIGH" : "LOW");
        }
    }

    static int get_pin_number(const char* pin_label) {
        // Simple implementation for native_posix - in real HW would use DT
        return 0; // Using pin 0 for both input and output
    }

    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
    const char* input_pin_;
    gpio_callback callback_;
    struct k_work work_;
    int last_state_ = -1;
    int64_t last_change_time_ = 0;
};

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
    ReactClass(const char* output_pin) : output_pin_(output_pin) {
        init();
        zbus_chan_add_obs(&gpio_state_change_chan, &observer_, K_NO_WAIT);
    }

    ~ReactClass() {
        zbus_chan_rm_obs(&gpio_state_change_chan, &observer_);
    }

private:
    void init() {
        const device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(output_gpio));
        if (!device_is_ready(gpio_dev)) {
            LOG_ERR("Output GPIO device not ready");
            return;
        }

        int ret = gpio_pin_configure(gpio_dev, get_pin_number(output_pin_), GPIO_OUTPUT);
        if (ret < 0) {
            LOG_ERR("Failed to configure output GPIO: %d", ret);
            return;
        }

        k_work_init_delayable(&blink_work_, blink_work_callback);
        LOG_INF("ReactClass initialized for pin %s", output_pin_);
    }

    static void observer_callback(const zbus_channel *chan) {
        gpio_state_change msg;
        zbus_chan_read(chan, &msg, K_NO_WAIT);

        ReactClass *self = CONTAINER_OF(chan, ReactClass, observer_);
        self->handle_gpio_change(msg.state);
    }

    void handle_gpio_change(bool state) {
        if (state) {
            // High state - blink 3 times
            blink_count_ = 0;
            k_work_schedule(&blink_work_, K_MSEC(100));
        } else {
            // Low state - turn on for 500ms
            const device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(output_gpio));
            gpio_pin_set(gpio_dev, get_pin_number(output_pin_), 1);
            k_work_schedule(&blink_work_, K_MSEC(500));
        }
    }

    static void blink_work_callback(struct k_work *work) {
        ReactClass *self = CONTAINER_OF(work, ReactClass, blink_work_);
        const device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(output_gpio));

        if (self->last_state_) {
            // Handle blinking for high state
            self->blink_count_++;
            if (self->blink_count_ <= 6) { // 3 on/off cycles
                bool led_state = (self->blink_count_ % 2) == 1;
                gpio_pin_set(gpio_dev, self->get_pin_number(self->output_pin_), led_state);
                if (self->blink_count_ < 6) {
                    k_work_schedule(&self->blink_work_, K_MSEC(100));
                }
            }
        } else {
            // Handle 500ms on for low state
            gpio_pin_set(gpio_dev, self->get_pin_number(self->output_pin_), 0);
        }
    }

    static int get_pin_number(const char* pin_label) {
        // Simple implementation for native_posix - in real HW would use DT
        return 1; // Using pin 1 for output (different from input pin 0)
    }

    const char* output_pin_;
    zbus_observer observer_ = {
        .chan = &gpio_state_change_chan,
        .queue = NULL,
        .callback = observer_callback
    };
    struct k_work_delayable blink_work_;
    bool last_state_ = false;
    uint8_t blink_count_ = 0;
};

// Define ZBus channel
ZBUS_CHAN_DEFINE(gpio_state_change_chan,
    gpio_state_change,
    NULL,
    NULL,
    ZBUS_OBSERVERS_EMPTY,
    ZBUS_MSG_INIT(0)
);

void main(void)
{
    ReadClass reader("input-gpio");
    ReactClass reactor("output-gpio");

    while (1) {
        k_sleep(K_MSEC(1000));
    }
}