#pragma once

#include "common.hpp"
namespace OSAL {

class osal_backend_t {
public:
	osal_backend_t() = default;
	~osal_backend_t() = default;

	virtual systime_t millis() = 0;
	virtual void delay(systime_t ms) = 0;
	virtual void assert(bool val, const char *reason) = 0;
};

class disp_backend_t {
public:
	disp_backend_t() = default;
	~disp_backend_t() = default;

	virtual void init() = 0;
};

}
