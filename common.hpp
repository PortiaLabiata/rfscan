#pragma once
#include "stm32f1xx.h"
#include <stddef.h>

enum class status {
	ok,
	err_arg,
};
using systime_t = uint32_t;

namespace HAL {

using status_t = status;

struct sys {
	static void init();
	static void assert(bool check, const char *reason);
	static systime_t millis();
	static void delay(systime_t ms);
};

extern size_t apb_presc_table[8U];

}
