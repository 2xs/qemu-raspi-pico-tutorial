cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(my_first_pico_code C CXX ASM)

pico_sdk_init()

add_executable(my_first_pico_code
    main.c
)

target_link_libraries(my_first_pico_code
    pico_stdlib
)

pico_enable_stdio_uart(my_first_pico_code 1)
pico_enable_stdio_usb(my_first_pico_code 0)

pico_add_extra_outputs(my_first_pico_code)
