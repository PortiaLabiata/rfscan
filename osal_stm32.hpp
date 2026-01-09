#pragma once
#include "osal_class.hpp"

using namespace HAL;

namespace OSAL {

class osal_stm32_t : public osal_backend_t {
public:
	osal_stm32_t() : osal_backend_t() {};
	~osal_stm32_t() {};

	void init() override;
	systime_t millis() override;
	void delay(systime_t ms) override;
	void assert(bool v, const char *reason) override;
};

}
