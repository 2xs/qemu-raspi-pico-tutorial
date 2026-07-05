#include <stdio.h>

#include "pico/multicore.h"
#include "pico/stdlib.h"

static void core1_entry(void)
{
    puts("hello from core 1");

    while (true) {
        tight_loop_contents();
    }
}

int main(void)
{
    stdio_init_all();

    multicore_launch_core1(core1_entry);
    puts("hello from core 0");

    while (true) {
        tight_loop_contents();
    }
}
