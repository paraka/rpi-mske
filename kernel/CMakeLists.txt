cmake_minimum_required(VERSION 2.8)

#############################
# Project globals
#############################
project(rpi-mske C ASM)
set(BINARY_NAME rpi-mske)

#############################
# includes
#############################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/klibc/include)

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
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel.ld)

# Set the linker flags so that we use our "custom" linker script
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")

file(GLOB KLIBC_SRC  "src/klibc/src/*.c")
set(BOOT_SRC "src/init/boot.s")
set(COMMON_SRC "src/common/device.c" 
               "src/common/panic.c")
set(GPIO_SRC "src/gpio/gpio.c")
set(LED_SRC "src/led/led.c")
set(UART_SRC "src/uart/uart.c")
set(IRQ_SRC "src/irq/irq.c")
set(EXCEPTIONS_SRC "src/exceptions/exceptions.c")
set(ARCH_ARM_SRC "src/arch/arm/processor.c")
set(TIMER_SRC "src/timer/timer.c")
set(ARM_TIMER_SRC "src/timer/arm_timer.c")
set(ATAGS_FILE   "src/init/atags.c")
set(KERNEL_MAIN_FILE "src/init/kernel.c")

set(OS_SRCS "src/os/task.c"
            "src/os/queue.c"
            "src/os/semaphore.c")

set(KERNEL_SOURCES 
    ${BOOT_SRC}
    ${COMMON_SRC}
    ${GPIO_SRC}
    ${LED_SRC}
    ${KLIBC_SRC}
    ${UART_SRC}
    ${IRQ_SRC}
    ${EXCEPTIONS_SRC}
    ${ARCH_ARM_SRC}
    ${TIMER_SRC}
    ${ARM_TIMER_SRC}
    ${ATAGS_FILE}
    ${OS_SRCS}
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
                COMMAND ${CMAKE_OBJCOPY} ${CMAKE_BINARY_DIR}/kernel/${BINARY_NAME} -O binary kernel.img
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Convert the ELF output file to a binary image")
