#pragma once
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"
#include "mutex_class.hpp"

namespace OS {

class mutex_stm32_t : public mutex_abstract_t {
private:
	SemaphoreHandle_t sem;
	StaticSemaphore_t buf;
public:
	mutex_stm32_t() {
		sem = xSemaphoreCreateMutexStatic(&buf);
		xSemaphoreGive(sem);
	}
	~mutex_stm32_t() {
		vSemaphoreDelete(sem);
	}

	void lock() override { xSemaphoreTake(sem, portMAX_DELAY); }
	void unlock() override { xSemaphoreGive(sem); }

};

}
