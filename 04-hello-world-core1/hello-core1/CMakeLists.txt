cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(hello_core1 C CXX ASM)

pico_sdk_init()

add_executable(hello_core1
    main.c
)

target_link_libraries(hello_core1
    pico_multicore
    pico_stdlib
)

pico_enable_stdio_uart(hello_core1 1)
pico_enable_stdio_usb(hello_core1 0)

pico_add_extra_outputs(hello_core1)
