cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(unique_board_id C CXX ASM)

pico_sdk_init()

add_executable(unique_board_id
    main.c
)

target_link_libraries(unique_board_id
    pico_stdlib
    pico_unique_id
)

pico_enable_stdio_uart(unique_board_id 1)
pico_enable_stdio_usb(unique_board_id 0)

pico_add_extra_outputs(unique_board_id)
