#pragma once
#include "FreeRTOS.h"
#include "task.h"
#include "sched_class.hpp"
#include "env.hpp"
#include "halconf.h"
#include "osal.hpp"

extern TASK_FUNC(main_task);
namespace OS {

class sched_stm32_t : public sched_abstract_t {
public:
	sched_stm32_t() = default;
	~sched_stm32_t() = default;

	void init() override {
		auto res = xTaskCreate(main_task, "main", 
						OS_MAIN_STACKS, nullptr, 1, &main_handle);
		osal->assert(res == pdPASS, "not allocated task");
		vTaskStartScheduler();
	}

	void create_task(const task_t& task) override {
		auto res = xTaskCreate(task.func, task.name, 
						task.stack_size, nullptr, task.prio, nullptr);
		osal->assert(res == pdPASS, "not allocated task");
	}
private:
	TaskHandle_t main_handle;
};

}
