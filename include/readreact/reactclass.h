#pragma once

#include <functional>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "readreact/zbusmanager.h"
#include "readreact/reactled.h"


/**
 * @class ReactClass
 * @brief Observes GPIO state changes and controls output GPIO accordingly.
 */
 class ReactClass : public BaseSubscriber {
public:
    /**
    * @brief Construct a new ReactClass object
    *
    */
    ReactClass(ReactLED *react_led);

    /**
    * @brief Handle state change of the GPIO.
    *
    * @param state The new state of the GPIO (high/low).
    */
    void handle_message(const struct ZBusMessage& msg) override;
    
private:
    ReactLED *react_led_{nullptr};

    // Define the work context structure
    struct WorkContext {
        struct k_work_delayable blink_work_;
        ReactClass *react_;
    };

    /**
    * @brief Initializes the class, sets up observers.
    */
    void init();

    /**
    * @brief Work callback for blinking LED when GPIO state is HIGH.
    *
    * @param work The work item for delayed execution.
    */
    static void blink_work_callback(struct k_work *work);
    bool last_state_;             /**< Last known GPIO state */

    WorkContext work_context_;           /**< work context */
};