#pragma once
#include <stddef.h>
#include "env.hpp"

namespace OS {

class sched_abstract_t {
public:
	sched_abstract_t() = default;
	virtual ~sched_abstract_t() {};

	struct task_t {
		TASK_FUNC((*func));
		const char *name;
		size_t prio;
		size_t stack_size;
	};

	virtual void init() = 0;
	virtual void create_task(const task_t& task) = 0;
};

}
