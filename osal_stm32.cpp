#include "osal_stm32.hpp"
#include "projdefs.h"
#include "stm32f103xb.h"
#include "spi.hpp"
#include "gpio.hpp"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {

void HardFault_Handler(void) {
	while (1) ;
}

void vApplicationStackOverflowHook(TaskHandle_t task, 
				char *name) {
	while (1) ;
}

}

namespace OSAL {

void osal_stm32_t::init() {
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_EnableIRQ(SysTick_IRQn);
	
	HAL::spidrv_t::init();
	HAL::ioline_t::init();
}

systime_t osal_stm32_t::millis() {
	return pdTICKS_TO_MS(xTaskGetTickCount());
}

void osal_stm32_t::delay(systime_t ms) {
	vTaskDelay(pdMS_TO_TICKS(ms));
}

void osal_stm32_t::assert(bool v, const char *reason) {
	if (!v) {
		while (1)
			;
	}
}

}

OSAL::osal_stm32_t osal_stm32;
