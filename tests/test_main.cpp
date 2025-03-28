#include <gtest/gtest.h>
#include <zephyr/kernel.h>
#include <nsi_main.h>

int main(void) {
    int argc = 1;
    char *argv = "test";

    ::testing::InitGoogleTest(&argc, &argv);
    int result = RUN_ALL_TESTS();
    nsi_exit(result);

    return 0;
}