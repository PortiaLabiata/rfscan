#pragma once
#include <SDL2/SDL.h>
#include "sched_class.hpp"
#include "halconf.h"

extern int main_task(void *args);
namespace OS {

class sched_pc_t : public sched_abstract_t {
public:
	sched_pc_t() = default;
	~sched_pc_t() = default;

	void init() override {
		SDL_CreateThreadWithStackSize(main_task, "main", 
						OS_MAIN_STACKS, nullptr);
	}

	void create_task(const task_t& task) override {
		SDL_CreateThreadWithStackSize(task.func, task.name,
						task.stack_size, nullptr);
	}
};

}
