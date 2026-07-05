/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Derived from pico-examples/system/unique_board_id.
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/unique_id.h"

int main(void)
{
    stdio_init_all();

    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    printf("Unique identifier:");
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; ++i) {
        printf(" %02x", board_id.id[i]);
    }
    printf("\n");

    return 0;
}
