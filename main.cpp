#include "osal.hpp"
#include "peripherals.hpp"
#include "sched.hpp"
#include "env.hpp"

using namespace OSAL;
using namespace OS;

#if !SITL
using namespace HAL;

TASK_FUNC(blink_task) {
	ioline_t led_line{GPIOC, 13U};
	led_line.cfg(linemode_t::out_pp);
	while (1) {
		led_line.toggle();
		osal->delay(500);
	}
}
#endif

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
		disp->dirty_stack.emplace(x, 0, 1, 16);
		disp->dirty_stack.last().fill(0x0000);
		disp->dirty_stack.emplace(x, 0, 16, 16);
		disp->dirty_stack.last().fill(
						dirty_t::encode_color(0, 255, 0));

		x++;
		if (x > disp->_sizex)
			x = 0;

		osal->delay(20);
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

#if !SITL
	sched->create_task({
		.func = blink_task,
		.name = "blink",
		.prio = 3,
		.stack_size = 64,
	});
#endif
	sched->init();

	while (1) {}
}
