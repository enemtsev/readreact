/**
 * @file readclass.h
 * @brief Definition of the ReadClass for monitoring GPIO input and publishing state changes
 */

#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "readreact/zbusmanager.h"

/**
 * @class ReadClass
 * @brief Handles GPIO input with debouncing and publishes state changes via ZBus
 *
 * This class configures a GPIO pin as input with interrupt capability,
 * implements debouncing logic to filter out spurious signals, and
 * publishes stable state changes to other components via ZBus.
 *
 * The class uses Zephyr's callback and work queue mechanisms to ensure
 * that GPIO interrupt handling is non-blocking and thread-safe.
 */
class ReadClass : public BasePublisher {
public:
    /**
     * @brief Constructs a new ReadClass instance
     */
    ReadClass();

    /**
     * @brief Destructor to clean up resources
     */
    ~ReadClass();

    /**
     * @brief Initialize the GPIO input and register callbacks
     *
     * @return true if initialization successful
     * @return false if initialization failed
     */
    bool init() override;

    /**
     * @brief Default debounce time in milliseconds
     *
     * Defines the minimum time required between state changes to be
     * considered valid (not bounces)
     */
    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;

private:
    /**
     * @brief Context structure for GPIO interrupt callback
     *
     * Contains the callback structure and a pointer back to the ReadClass instance
     */
    struct CallbackContext {
        struct gpio_callback callback_; /**< GPIO callback structure */
        ReadClass *read_;               /**< Pointer back to ReadClass instance */
    };

    /**
     * @brief Context structure for work queue callback
     *
     * Contains the work item and a pointer back to the ReadClass instance
     */
    struct WorkContext {
        struct k_work_delayable work_; /**< Delayable work item */
        ReadClass *read_;              /**< Pointer back to ReadClass instance */
    };

    /**
     * @brief GPIO interrupt callback function
     *
     * Called when the input GPIO changes state. Schedules the work
     * item to process the change in a non-ISR context.
     *
     * @param dev GPIO device that triggered the interrupt
     * @param cb Callback structure containing context
     * @param pins Bitmask of pins that triggered the interrupt
     */
    static void interrupt_callback(const device *dev, gpio_callback *cb, uint32_t pins);

    /**
     * @brief Work queue callback function
     *
     * Processes the GPIO state change in a thread context, allowing for
     * more complex operations that would be unsafe in an ISR.
     *
     * @param work Pointer to the work item
     */
    static void work_callback(struct k_work *work);

    /**
     * @brief Processes a detected GPIO change with debouncing
     *
     * Reads the current GPIO state, applies debouncing logic, and
     * publishes state changes over ZBus when appropriate.
     */
    void process_gpio_change();

    /** @brief Context for GPIO interrupt callback */
    CallbackContext callback_context_;

    /** @brief Context for work queue callback */
    WorkContext work_context_;

    /** @brief Last stable state of the GPIO (-1 = uninitialized) */
    int last_state_ = -1;

    /** @brief Timestamp of the last valid state change (for debouncing) */
    int64_t last_change_time_ = 0;

protected:
    /** @brief Pointer to the GPIO device */
    const device *gpio_dev_{nullptr};

    /** @brief GPIO pin number */
    gpio_pin_t pin_{0};

    /** @brief GPIO pin configuration flags */
    gpio_flags_t flags_;

    /**
     * @brief Initialize the GPIO device for input
     *
     * @return true if initialization successful
     * @return false if initialization failed
     */
    bool init_gpio_device();
};