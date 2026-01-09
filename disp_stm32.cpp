#include "disp_stm32.hpp"

namespace OSAL {

void disp_stm32_t::send_command(uint8_t *buf, size_t size) {
	set_command();
	for (size_t i = 0; i < size; i++) {
		spi.write(buf[i]);
	}
}

void disp_stm32_t::send_data(uint8_t *buf, size_t size) {
	set_data();
	for (size_t i = 0; i < size; i++) {
		spi.write(buf[i]);
	}
}

void disp_stm32_t::init() {
	spi.cfg(1000000);
	rst_line.cfg(linemode_t::out_pp);
	dc_line.cfg(linemode_t::out_pp);
}

void disp_stm32_t::flush() {
	
}

}
