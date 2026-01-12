cmake_minimum_required(VERSION 3.13)
project(rfscan LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

# Configure environment file, it contains 
# SITL define
option(SITL "Build fot SITL environment" OFF)
configure_file(env.hpp.in env.hpp)
include_directories(${PROJECT_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT SITL)
	# Add startup and CMSIS system files
	add_subdirectory(bpill)

	# Theese have to go before everything else, in case some 
	# submodule decides that it needs to check compiler, which will
	# not work, as we did not yet set compiler specs to nosys.

	set(TOOLCHAIN /home/portia_labiata/.platformio/packages/toolchain-gccarmnoneeabi/bin)
	set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/arm-none-eabi-g++)
	set(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
	set(CMAKE_ASM_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)

	# Mostly options to reduce .text size.
	set(UC_COMPILE_OPTIONS -Wall -Og -mcpu=cortex-m3 
			-mthumb -mno-thumb-interwork -nostdlib --specs=nosys.specs
			-flto)
	set(UC_LINK_OPTIONS 
		-Wl,-L${CMAKE_CURRENT_SOURCE_DIR}/bpill,-T./STM32F103XB_FLASH.ld)

	# Theese have to go before FreeRTOS compilation so that compiler
	# knows we are using Thumb mode and specific linker script.

	add_compile_options(${UC_COMPILE_OPTIONS})
	add_link_options(${UC_LINK_OPTIONS})

	set(FREERTOS_PORT GCC_ARM_CM3)
	set(FREERTOS_HEAP "1" CACHE STRING "" FORCE)

	add_library(freertos_config INTERFACE)
	target_include_directories(freertos_config SYSTEM INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(FreeRTOS-Kernel)
endif()

set(COMMON_SOURCES main.cpp common.cpp)
add_compile_options(-fno-exceptions -fno-rtti)
if(NOT SITL)
	set(ALL_SOURCES ${COMMON_SOURCES} gpio.cpp spi.cpp osal_stm32.cpp
			disp_stm32.cpp)

	# There are two versions of firmware: regular and debug. In regular
	# version, all debug symbols are stripped, which cuts .text size
	# almost in half. But it makes debugging virtually impossible, so
	# to actually see what we are doing, we need debug version.

	add_executable(${PROJECT_NAME}_debug.elf ${ALL_SOURCES})
	target_link_libraries(${PROJECT_NAME}_debug.elf CMSIS freertos_kernel)

	add_executable(${PROJECT_NAME}.elf ${ALL_SOURCES})
	target_link_libraries(${PROJECT_NAME}.elf CMSIS freertos_kernel)
	target_link_options(${PROJECT_NAME}.elf PUBLIC
			-Wl,--strip-all)
else()
	# Since SDL2 is cross-platform and original purpose of SITL
	# is to debug graphics code, it is used as a framework for 
	# video output and threading, possibly (later) for networking.

	# Here we do not need to save space, so no stripped version.

	find_package(SDL2 REQUIRED)
	add_executable(sitl ${COMMON_SOURCES} osal_pc.cpp disp_pc.cpp)
	target_link_libraries(sitl SDL2::SDL2)
	target_compile_options(sitl PUBLIC -g)
endif()
