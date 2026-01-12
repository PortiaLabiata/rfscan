add_library(CMSIS OBJECT)
enable_language(ASM C)

set(TOOLCHAIN 
		/home/portia_labiata/.platformio/packages/toolchain-gccarmnoneeabi/bin)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/arm-none-eabi-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)

# Show CMSIS, which chip we are using.

target_compile_definitions(CMSIS PUBLIC STM32F103xB)
target_include_directories(CMSIS 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(UC_COMPILE_OPTIONS -Wall -Og -mcpu=cortex-m3 
		-mthumb -mno-thumb-interwork -g)
set(UC_LINK_OPTIONS 
		-Wl,-L${CMAKE_CURRENT_SOURCE_DIR},-T./STM32F103XB_FLASH.ld)

target_compile_options(CMSIS PUBLIC ${UC_COMPILE_OPTIONS})
target_link_options(CMSIS PUBLIC 
		${UC_COMPILE_OPTIONS} ${UC_LINK_OPTIONS})

target_sources(CMSIS PRIVATE startup_stm32f103xb.s)
# Syscalls.c file is taken from ChibiOS sources.
target_sources(CMSIS PRIVATE system_stm32f1xx.c syscalls.c)
target_link_libraries(CMSIS PUBLIC libarm_cortexM3l_math.a)
