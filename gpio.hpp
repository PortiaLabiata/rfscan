#pragma once
#include "stm32f103xb.h"
#include "common.hpp"

namespace HAL {

enum class linemode_t {
	out_pp 		= 0b0000 | 0b0011,
	out_od 		= 0b0100 | 0b0011,
	af_pp 		= 0b1000 | 0b0011,
	af_od 		= 0b1100 | 0b0011,
	in_pup 		= 0b1000 | 0b0000,
	in_pdown	= 0b1000 | 0b0000,
	in_float 	= 0b0100 | 0b0000,
	in_analog 	= 0b0000 | 0b0000,
};

struct ioline_t {
	GPIO_TypeDef *port;
	uint32_t pin;

	static void init();
	status_t cfg(linemode_t mode);
	void set();
	void clear();
	void toggle();
};

}
