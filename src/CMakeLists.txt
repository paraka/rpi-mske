cmake_minimum_required(VERSION 2.8)

#############################
# Project globals
#############################
project(rpi-mske C ASM)
set(BINARY_NAME rpi-mske)

#############################
# includes
#############################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/klibc/include)

#############################
# Definitions
#############################
string(TIMESTAMP KERNEL_BUILD_TIME %Y%m%d_%I%M%S UTC)
message(STATUS "Building at ${KERNEL_BUILD_TIME}")

#############################
# Flags
#############################
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra -nostdlib")

#############################
# Sources
#############################
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
# Set the linker flags so that we use our "custom" linker script
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")

file(GLOB KLIBC_SRC  "klibc/src/*.c")
set(BOOT_SRC "init/boot.s")
set(ENTRY_SRC "init/entry.S")
set(COMMON_SRC "common/device.c" 
               "common/panic.c")
set(GPIO_SRC "gpio/gpio.c")
set(LED_SRC "led/led.c")
set(UART_SRC "uart/uart.c")
set(IRQ_SRC "irq/irq.c")
set(EXCEPTIONS_SRC "exceptions/exceptions.c")
set(ARCH_ARM_SRC "arch/arm/processor.c")
set(TIMER_SRC "timer/timer.c")
set(ATAGS_FILE   "init/atags.c")
set(KERNEL_MAIN_FILE "init/kernel.c")

set(KERNEL_SOURCES 
    ${BOOT_SRC}
    ${ENTRY_SRC}
    ${COMMON_SRC}
    ${GPIO_SRC}
    ${LED_SRC}
    ${KLIBC_SRC}
    ${UART_SRC}
    ${IRQ_SRC}
    ${EXCEPTIONS_SRC}
    ${ARCH_ARM_SRC}
    ${TIMER_SRC}
    ${ATAGS_FILE}
    ${KERNEL_MAIN_FILE})

add_executable(${BINARY_NAME} ${KERNEL_SOURCES})

#############################
# Libraries
#############################
target_link_libraries(${BINARY_NAME} -lgcc)

#############################
# Other customs
#############################
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
                COMMAND ${CMAKE_OBJCOPY} ${CMAKE_BINARY_DIR}/src/${BINARY_NAME} -O binary kernel.img
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Convert the ELF output file to a binary image")
