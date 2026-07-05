#include <stdio.h>

#include "pico/stdlib.h"

int main(void)
{
    stdio_init_all();

    while (true) {
        puts("my own hello world");
        sleep_ms(1000);
    }
}
