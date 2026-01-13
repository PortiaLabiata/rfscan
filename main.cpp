#include "mutex_class.hpp"
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
		osal->delay(100);
	}
}

TASK_FUNC(main_task) {
	const size_t size = 16;
	size_t x = size;
	osal->delay(2000);
	while (1) {
#if SITL == ON
		auto *disp_pc = static_cast<disp_pc_t*>(disp);
		if (disp_pc->deinit_flag)
			disp_pc->deinit();
#endif
		// Will slow things down a bit, but works for now
		{
		OS::lock_guard_t guard(disp->dirty_queue);
		auto *block = disp->dirty_queue.emplace(x, 0, size, 16);
		auto* clear = disp->dirty_queue.emplace(x-16, 0, size, 16);

		if (block)
			block->fill(dirty_t::encode_color(0, 255, 0));

		if (clear)
			clear->fill(0x0000);
		}

		x+=size;
		if (x > disp->_sizex)
			x = size;

		disp->printf(32, 32, "H E L L O");

		osal->delay(1000);
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
