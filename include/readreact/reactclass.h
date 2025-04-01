/**
 * @file reactclass.h
 * @brief Definition of the ReactClass for handling GPIO output based on input state
 */

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <functional>

#include "readreact/reactled.h"
#include "readreact/zbusmanager.h"

/**
 * @class ReactClass
 * @brief Observes GPIO state changes via ZBus and controls output GPIO (LED) accordingly.
 *
 * This class serves as a subscriber to GPIO state change messages published via ZBus.
 * Based on the received state, it implements different LED behaviors:
 * - For HIGH state: blinks the LED 3 times with 100ms intervals
 * - For LOW state: turns the LED on for 500ms
 *
 * The class uses a delayed work queue to implement the timing patterns,
 * ensuring that the LED control doesn't block the ZBus observer callback.
 */
class ReactClass : public BaseSubscriber {
public:
    /**
     * @brief Construct a new ReactClass object
     *
     * @param react_led Pointer to a ReactLED object for controlling the output
     */
    ReactClass(ReactLED *react_led);

    /**
     * @brief Handle state change messages from ZBus
     *
     * This method implements the BaseSubscriber interface and is called
     * when new messages are published on the ZBus channel.
     *
     * @param msg The ZBus message containing the new GPIO state
     */
    void handle_message(const struct ZBusMessage &msg) override;

private:
    /** @brief Pointer to the LED controller */
    ReactLED *react_led_{nullptr};

    /**
     * @brief Work context structure for delayed work execution
     *
     * Contains the work item and a pointer back to the ReactClass instance
     */
    struct WorkContext {
        struct k_work_delayable blink_work_; /**< Delayable work item */
        ReactClass *react_;                  /**< Pointer to ReactClass instance */
    };

    /**
     * @brief Initializes the class, sets up work item
     */
    void init();

    /**
     * @brief Static work callback for controlling LED patterns
     *
     * This callback implements the LED blinking patterns based on the last GPIO state.
     * The function is static as required by Zephyr's work queue API, and uses
     * the WorkContext to access the ReactClass instance.
     *
     * @param work Pointer to the work item being executed
     */
    static void blink_work_callback(struct k_work *work);

    /**
     * @brief Last known GPIO state from ZBus
     *
     * true = HIGH, false = LOW
     */
    bool last_state_;

    /** @brief Work context for LED control */
    WorkContext work_context_;
};