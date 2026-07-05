#include <stdio.h>
#include <string.h>

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"

#define COUNTER_MAGIC 0x52435051u

struct counter_record {
    uint32_t magic;
    uint32_t value;
};

static const uint8_t counter_sector[FLASH_SECTOR_SIZE]
    __attribute__((aligned(FLASH_SECTOR_SIZE), section(".rodata.counter_sector"))) = {
        [0] = (uint8_t)(COUNTER_MAGIC),
        [1] = (uint8_t)(COUNTER_MAGIC >> 8),
        [2] = (uint8_t)(COUNTER_MAGIC >> 16),
        [3] = (uint8_t)(COUNTER_MAGIC >> 24),
};

static uint8_t page[FLASH_PAGE_SIZE];

static uint32_t counter_flash_offset(void)
{
    uintptr_t xip_addr = (uintptr_t)counter_sector;

    return (uint32_t)(xip_addr - XIP_BASE);
}

int main(void)
{
    const volatile struct counter_record *record =
        (const volatile void *)counter_sector;
    struct counter_record next = {
        .magic = COUNTER_MAGIC,
        .value = 1,
    };
    uint32_t flash_offs;
    uint32_t ints;

    stdio_init_all();

    if (record->magic == COUNTER_MAGIC) {
        next.value = record->value + 1;
    }

    printf("reboot counter: %lu\n", (unsigned long)next.value);

    memset(page, 0xff, sizeof(page));
    memcpy(page, &next, sizeof(next));

    flash_offs = counter_flash_offset();
    ints = save_and_disable_interrupts();
    flash_range_erase(flash_offs, FLASH_SECTOR_SIZE);
    flash_range_program(flash_offs, page, FLASH_PAGE_SIZE);
    restore_interrupts(ints);

    while (true) {
        tight_loop_contents();
    }
}
