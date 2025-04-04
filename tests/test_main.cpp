#include <gtest/gtest.h>
#include <nsi_main.h>
#include <zephyr/kernel.h>

int main(void) {
    int argc = 1;
    char *argv = "test";

    // let clock to start
    k_sleep(K_MSEC(1000));

    ::testing::InitGoogleTest(&argc, &argv);
    int result = RUN_ALL_TESTS();
    nsi_exit(result);

    return 0;
}