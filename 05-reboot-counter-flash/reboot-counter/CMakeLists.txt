cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(reboot_counter C CXX ASM)

pico_sdk_init()

add_executable(reboot_counter
    main.c
)

target_link_libraries(reboot_counter
    hardware_flash
    hardware_sync
    pico_stdlib
)

pico_enable_stdio_uart(reboot_counter 1)
pico_enable_stdio_usb(reboot_counter 0)

pico_add_extra_outputs(reboot_counter)
