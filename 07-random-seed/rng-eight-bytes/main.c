/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Uses the Pico SDK pico_rand API, following the spirit of
 * pico-examples/system/rand but keeping the output short for the tutorial.
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "pico/rand.h"
#include "pico/stdlib.h"

int main(void)
{
    stdio_init_all();

    uint64_t value = get_rand_64();

    printf("Random bytes:");
    for (int i = 7; i >= 0; i--) {
        printf(" %02" PRIx8, (uint8_t)(value >> (i * 8)));
    }
    printf("\n");

    return 0;
}
