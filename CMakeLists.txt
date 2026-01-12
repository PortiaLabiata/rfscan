cmake_minimum_required(VERSION 3.13)
project(rfscan LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

option(SITL "Build fot SITL environment" OFF)
configure_file(env.hpp.in env.hpp)
include_directories(${PROJECT_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT SITL)
	add_subdirectory(bpill)

	set(TOOLCHAIN /home/portia_labiata/.platformio/packages/toolchain-gccarmnoneeabi/bin)
	set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/arm-none-eabi-g++)
	set(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
	set(CMAKE_ASM_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)

	set(UC_COMPILE_OPTIONS -Wall -O0 -mcpu=cortex-m3 
		-mthumb -mno-thumb-interwork -g)
	set(UC_LINK_OPTIONS 
		-Wl,-L${CMAKE_CURRENT_SOURCE_DIR}/bpill,-T./STM32F103XB_FLASH.ld)

	set(FREERTOS_PORT GCC_ARM_CM3)
	add_compile_options(${UC_COMPILE_OPTIONS})
	add_link_options(${UC_LINK_OPTIONS})
	set(FREERTOS_HEAP "2" CACHE STRING "" FORCE)
	add_library(freertos_config INTERFACE)
	target_include_directories(freertos_config SYSTEM INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(FreeRTOS-Kernel)
endif()

set(COMMON_SOURCES main.cpp common.cpp)
if(NOT SITL)
	add_executable(${PROJECT_NAME}.elf ${COMMON_SOURCES} gpio.cpp
		spi.cpp osal_stm32.cpp disp_stm32.cpp)
	target_link_libraries(${PROJECT_NAME}.elf CMSIS freertos_kernel)
else()
	find_package(SDL2 REQUIRED)
	add_executable(sitl ${COMMON_SOURCES} osal_pc.cpp disp_pc.cpp)
	target_link_libraries(sitl SDL2::SDL2)
	target_compile_options(sitl PUBLIC -g)
endif()
