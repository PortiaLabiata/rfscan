#include "osal.hpp"
#include "peripherals.hpp"
#include "sched.hpp"
#include "env.hpp"

using namespace OSAL;
using namespace OS;

TASK_FUNC(video_task) {
	disp->init();
	disp->clear();
	while (1) {
		disp->flush();
		osal->delay(20);
	}
}

TASK_FUNC(main_task) {
	size_t x = 0;
	while (1) {
#if SITL == ON
		auto *disp_pc = static_cast<disp_pc_t*>(disp);
		if (disp_pc->deinit_flag)
			disp_pc->deinit();
#endif
		disp->dirty_stack.emplace({x++, 0, 16, 16});
		if (x > disp->_sizex)
			x = 0;

		auto& area = disp->dirty_stack.last();
		area.fill(dirty_t::encode_color(0, 255, 0));
		osal->delay(5);
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
