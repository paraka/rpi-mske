cmake_minimum_required(VERSION 2.8)

project(rpi-mske C ASM)

set(BINARY_NAME rpi-mske)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/klibc/include)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra -nostdlib")

# Set the linker flags so that we use our "custom" linker script
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")

set(BOOT_SRC "init/boot.s")
set(COMMON_SRC "common/device.c" 
               "common/panic.c")
set(GPIO_SRC "gpio/gpio.c")
set(LED_SRC "led/led.c")
set(UART_SRC "uart/uart.c")
set(KERNEL_MAIN_FILE "init/kernel.c")

set(KERNEL_SOURCES 
    ${BOOT_SRC}
    ${COMMON_SRC}
    ${GPIO_SRC}
    ${LED_SRC}
    ${UART_SRC}
    ${KERNEL_MAIN_FILE})

add_executable(${BINARY_NAME} ${KERNEL_SOURCES})

add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
                COMMAND ${CMAKE_OBJCOPY} ${CMAKE_BINARY_DIR}/src/${BINARY_NAME} -O binary kernel.img
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Convert the ELF output file to a binary image")
