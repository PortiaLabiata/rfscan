#include "osal.hpp"
#include "peripherals.hpp"

int main(void) {
	osal->init();
	disp->init();

	while (1) {
		disp->flush();
		osal->delay(20);
	}
		
}
