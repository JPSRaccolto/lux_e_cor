# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(lux_cor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(lux_cor lux_cor.c lib/bh1750_light_sensor.c lib/ssd1306.c)

pico_set_program_name(lux_cor "lux_cor")
pico_set_program_version(lux_cor "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(lux_cor 1)
pico_enable_stdio_usb(lux_cor 1)

# Add the standard library to the build
target_link_libraries(lux_cor pico_stdlib hardware_i2c hardware_pio)

# Add the standard include files to the build
target_include_directories(lux_cor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/lib
)


pico_add_extra_outputs(lux_cor)

