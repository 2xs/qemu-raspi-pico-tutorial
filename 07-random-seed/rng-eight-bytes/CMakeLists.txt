cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(rng_eight_bytes C CXX ASM)

pico_sdk_init()

add_executable(rng_eight_bytes
    main.c
)

target_link_libraries(rng_eight_bytes
    pico_rand
    pico_stdlib
    pico_time
)

target_compile_definitions(rng_eight_bytes PRIVATE
    PICO_RAND_ENTROPY_SRC_ROSC=0
    PICO_RAND_SEED_ENTROPY_SRC_ROSC=1
    PICO_RAND_ENTROPY_SRC_TIME=0
    PICO_RAND_SEED_ENTROPY_SRC_TIME=0
    PICO_RAND_ENTROPY_SRC_BUS_PERF_COUNTER=0
    PICO_RAND_SEED_ENTROPY_SRC_BUS_PERF_COUNTER=0
    PICO_RAND_SEED_ENTROPY_SRC_BOARD_ID=0
    PICO_RAND_SEED_ENTROPY_SRC_RAM_HASH=0
)

target_compile_options(rng_eight_bytes PRIVATE
    "$<$<COMPILE_LANGUAGE:C>:-include>"
    "$<$<COMPILE_LANGUAGE:C>:pico/time.h>"
)

pico_enable_stdio_uart(rng_eight_bytes 1)
pico_enable_stdio_usb(rng_eight_bytes 0)

pico_add_extra_outputs(rng_eight_bytes)
