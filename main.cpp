#include "osal.hpp"
#include "peripherals.hpp"
#include "sched.hpp"
#include "env.hpp"

using namespace OSAL;
using namespace OS;

TASK_FUNC(video_task) {
	disp->init();
	while (1) {
		disp->flush();
		osal->delay(20);
	}
}

TASK_FUNC(main_task) {
	while (1) {
		disp->dirty_stack.emplace({0, 0, 16, 16});
		auto& area = disp->dirty_stack.last();
		area.fill(dirty_t::encode_color(0, 255, 0));
		osal->delay(100);
	}
}

int main(void) {
	osal->init();

	sched->create_task({
		.func = video_task,
		.name = "video",
		.prio = 1,
		.stack_size = 512,
	});
	sched->init();

	while (1) {}
}
