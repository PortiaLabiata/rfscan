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
	ioline_t miso_line{GPIOA, 5U};
	ioline_t mosi_line{GPIOA, 7U};

	miso_line.cfg(linemode_t::af_pp);
	mosi_line.cfg(linemode_t::af_pp);

	spi.cfg(2000000);

	rst_line.cfg(linemode_t::out_pp);
	dc_line.cfg(linemode_t::out_pp);

	hard_reset();
	osal->delay(150);
	// Sleep out
	send_commandb(0x11);
	osal->delay(100);
	// Inv on
	send_commandb(0x21);
	osal->delay(100);
	// Disp ON
	send_commandb(0x29);
	osal->delay(10);

	send_commandb(0x3A);
	send_datab(0b01010101);
	osal->delay(10);

	/*
	send_commandb(0x36);
	send_datab(1 << 5);
	osal->delay(10);
	*/
}

void disp_stm32_t::_draw_dirty(const dirty_t& area) {
	const auto end_x = area.x + area.size_x;
	const auto end_y = area.y + area.size_y;

	set_column_mode(area.x, end_x);
	set_row_mode(area.y, end_y);

	send_commandb(0x2C);

	for (auto i = area.x; i < end_x; i++) {
		for (auto j = area.y; j < end_y; j++) {
			const auto pixel = area.buffer[i][j];	
			auto pixel_data = ((pixel >> 8) & 0xFF) |
					((pixel & 0xFF) << 8);
			send_data(reinterpret_cast<uint8_t*>(&pixel_data), 2);
		}
	}
	send_commandb(0x00);
}

void disp_stm32_t::clear() {
	set_column_mode(0, _sizex);
	set_row_mode(0, _sizey);

	send_commandb(0x2C);

	for (size_t i = 0; i < _sizex; i++) {
		for (size_t j = 0; j < _sizey; j++) {
			send_datab(0x00);
			send_datab(0x00);
		}
	}
	send_commandb(0x00);
}

void disp_stm32_t::flush() {
	while (dirty_stack.size() > 0) {
		const auto area = dirty_stack.pop();
		_draw_dirty(area);
	}
}

}
