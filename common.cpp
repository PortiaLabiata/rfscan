#include "common.hpp"

static systime_t _ticks = 0;

namespace HAL {

void sys::init() {
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_EnableIRQ(SysTick_IRQn);
}

systime_t sys::millis() {
	return _ticks;
}

void sys::delay(systime_t ms) {
	const auto now = millis();
	while (millis() - now <= ms) {
		asm volatile ("nop");
	}
}

void sys::assert(bool check, const char *reason) {
#if DEBUG_ASSERTS == ON
	if (!check) {
		while (1)
			;
	}
#endif
}
size_t apb_presc_table[8] = {1, 1, 1, 1, 1, 2, 3, 4};

}

extern "C" {

void SysTick_Handler(void) {
	_ticks++;
}

}
