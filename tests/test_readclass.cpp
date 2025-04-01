#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "readreact/readclass.h"
#include "zephyr/drivers/gpio.h"

#include <zephyr/drivers/gpio/gpio_emul.h>

#define DEV_IN DT_GPIO_CTLR(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN DT_GPIO_PIN(DT_INST(0, test_gpio_basic_api), in_gpios)
#define PIN_IN_FLAGS DT_GPIO_FLAGS(DT_INST(0, test_gpio_basic_api), in_gpios)

class ReadClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        gpio_dev = DEVICE_DT_GET(DEV_IN);
        ASSERT_TRUE(device_is_ready(gpio_dev));
    }

    void TearDown() override {
        gpio_pin_set(gpio_dev, PIN_IN, 0);
    }

    const struct device *gpio_dev;
};

// TEST_F(ReadClassTest, InitializesCorrectly) {
//     ReadClass read;

    
    
//     // Verify the ReadClass has initialized properly
//     EXPECT_TRUE(device_is_ready(gpio_dev));
// }

TEST_F(ReadClassTest, DetectsHighSignal) {
    // const struct device *gpio_dev;
    // int ret;

    // gpio_dev = DEVICE_DT_GET(GPIO_EMUL_NODE);
    // if (!device_is_ready(gpio_dev)) {
    //     printk("Error: GPIO emulator device not ready\n");
    //     return;
    // }

    // ret = gpio_pin_configure(gpio_dev, TEST_PIN, GPIO_OUTPUT_ACTIVE);
    // if (ret < 0) {
    //     printk("Failed to configure GPIO pin %d\n", TEST_PIN);
    //     return;
    // }

    // printk("GPIO emulator initialized. Toggling GPIO pin...\n");

    // while (1) {
    //     gpio_pin_toggle(gpio_dev, TEST_PIN);
        
    //     int v = gpio_emul_output_get(gpio_dev, TEST_PIN);
    //     printk("Toggled GPIO pin %d %d\n", TEST_PIN, v);
        
    //     k_msleep(1000);
    // }


    // const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(gpio_emul));

    // /* Configure pin 1 as input */
	// gpio_pin_configure(dev, 1, GPIO_INPUT);

	// /* Read pin 1 */
	// gpio_pin_set(dev, 1, 1);
	// volatile int v = gpio_pin_get(dev, 1);
	// gpio_pin_set(dev, 1, 0);
	// v = gpio_pin_get(dev, 1);

    // int callback_count{0};
    // ReadClass read;
    // read.set_publish_callback([&callback_count](const ZBusMessage &){
    //     ++callback_count;
    // });
    
    // // Simulate a HIGH signal on the GPIO pin
    // // gpio_pin_set(gpio_dev, PIN_IN, 1);
    
    // // Wait for debounce period plus a little extra
    // k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));
    
    // gpio_pin_set(gpio_dev, PIN_IN, 0);

    // EXPECT_EQ(callback_count, 1);
}

// TEST_F(ReadClassTest, DetectsLowSignal) {
//     int callback_count{0};
//     ReadClass read;
//     read.set_publish_callback([&callback_count](const ZBusMessage &){
//         ++callback_count;
//     });
    
//     // First set to HIGH so we can detect a LOW transition
//     gpio_pin_set(gpio_dev, PIN_IN, 1);
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

//     EXPECT_EQ(callback_count, 1);
//     callback_count = 0;
    
//     // Now simulate a LOW signal
//     gpio_pin_set(gpio_dev, PIN_IN, 0);
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

//     EXPECT_EQ(callback_count, 1);
// }

// TEST_F(ReadClassTest, DebouncesProperly) {
//     int callback_count{0};
//     ReadClass read;
//     read.set_publish_callback([&callback_count](const ZBusMessage &){
//         ++callback_count;
//     });
    
//     // Simulate multiple HIGH signals in quick succession (within debounce time)
//     gpio_pin_set(gpio_dev, PIN_IN, 1);
//     k_sleep(K_MSEC(10));  // Less than debounce time
//     gpio_pin_set(gpio_dev, PIN_IN, 0);
//     k_sleep(K_MSEC(10));  // Less than debounce time
//     gpio_pin_set(gpio_dev, PIN_IN, 1);
    
//     // Wait for debounce period plus a little extra
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));
    
//     EXPECT_EQ(callback_count, 1);
// }

// TEST_F(ReadClassTest, HandlesMultipleStateChanges) {
//     int callback_count{0};
//     ReadClass read;
//     read.set_publish_callback([&callback_count](const ZBusMessage &){
//         ++callback_count;
//     });
    
//     // First transition: LOW to HIGH
//     gpio_pin_set(gpio_dev, PIN_IN, 1);
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));
    
//     // Second transition: HIGH to LOW
//     gpio_pin_set(gpio_dev, PIN_IN, 0);
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));
    
//     // Third transition: LOW to HIGH
//     gpio_pin_set(gpio_dev, PIN_IN, 1);
//     k_sleep(K_MSEC(ReadClass::DEBOUNCE_TIME_MS + 10));

//     EXPECT_EQ(callback_count, 3);
// }