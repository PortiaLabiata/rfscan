#pragma once
#include <SDL2/SDL.h>
#include "osal_class.hpp"

namespace OSAL {

class osal_pc_t : public osal_backend_t {
public:
	osal_pc_t() : osal_backend_t() {};
	~osal_pc_t() {};

	void init() override;
	systime_t millis() override;
	void delay(systime_t ms) override;
	void assert(bool v, const char *reason) override;
private:
	systime_t _start_ms;
};

}
