#include "osal.hpp"
#include "osal_class.hpp"
#include "peripherals.hpp"

using namespace OSAL;

int main(void) {
	osal->init();
	disp->init();

	disp->clear();
	while (1) {
		disp->dirty_stack.emplace({0, 0, 16, 16});
		auto area = disp->dirty_stack.last();
		area.fill(dirty_t::encode_color(255, 0, 0));

		disp->flush();
		osal->delay(20);
	}
		
}
