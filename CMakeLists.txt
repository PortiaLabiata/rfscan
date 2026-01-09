cmake_minimum_required(VERSION 3.13)
project(rfscan LANGUAGES CXX)

set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(UC_COMPILE_OPTIONS -Wall -Og -mcpu=cortex-m3 
		-mthumb -mno-thumb-interwork -g)
set(UC_LINK_OPTIONS 
		-Wl,-L${CMAKE_CURRENT_SOURCE_DIR}/bpill,-T./STM32F103XB_FLASH.ld)

add_subdirectory(bpill)
add_executable(${PROJECT_NAME}.elf main.cpp common.cpp gpio.cpp
	spi.cpp)
target_link_libraries(${PROJECT_NAME}.elf CMSIS)
target_compile_options(${PROJECT_NAME}.elf PUBLIC ${UC_COMPILE_OPTIONS})
target_link_options(${PROJECT_NAME}.elf PUBLIC
		${UC_COMPILE_OPTIONS} ${UC_LINK_OPTIONS})
