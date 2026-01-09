#include "osal_stm32.hpp"
#include "stm32f103xb.h"

extern "C" {

static systime_t _ticks = 0;
void SysTick_Handler(void) {
	_ticks++;
}

}

namespace OSAL {

void osal_stm32_t::init() {
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_EnableIRQ(SysTick_IRQn);
}

systime_t osal_stm32_t::millis() {
	return _ticks;
}

void osal_stm32_t::delay(systime_t ms) {
	const auto now_ms = millis();
	while (millis() - now_ms <= ms)
		asm volatile ("nop");
}

void osal_stm32_t::assert(bool v, const char *reason) {
	if (!v) {
		while (1)
			;
	}
}

}

OSAL::osal_stm32_t osal_stm32;
