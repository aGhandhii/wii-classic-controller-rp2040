# Define min cmake version
cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(wiiClassic)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# This will be made into a .uf2
add_executable(${PROJECT_NAME}
    main.c
)
    
# pull in common dependencies
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_binary_info hardware_i2c)

# Enable USB out and disable UART out
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})