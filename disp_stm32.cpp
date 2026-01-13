#include "disp_stm32.hpp"
#include "font.hpp"

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

	send_commandb(0x36);
	send_datab(1 << 5);
	osal->delay(10);
}

void disp_stm32_t::_draw_dirty(const dirty_t& area) {
	const auto end_x = area.x + area.size_x;
	const auto end_y = area.y + area.size_y;

	set_column_mode(area.y, end_y-1);
	set_row_mode(area.x, end_x-1);

	send_commandb(0x2C);

	for (size_t i = 0; i < area.size_x; i++) {
		for (size_t j = 0; j < area.size_y; j++) {
			const auto pixel = area.buffer[i][j];	
			auto pixel_data = ((pixel >> 8) & 0xFF) |
					((pixel & 0xFF) << 8);
			send_data(reinterpret_cast<uint8_t*>(&pixel_data), 2);
		}
	}
	send_commandb(0x00);
}

void disp_stm32_t::clear() {
	set_column_mode(0, _sizex-1);
	set_row_mode(0, _sizey-1);

	send_commandb(0x2C);

	for (size_t i = 0; i < _sizex; i++) {
		for (size_t j = 0; j < _sizey; j++) {
			send_datab(0x00);
			send_datab(0x00);
		}
	}
	send_commandb(0x00);
}

size_t font_lookup(char c) {
	for (size_t i = 0; i < sizeof(Draw::char_map)/sizeof(Draw::char_t); 
					i++) {
		if (c == Draw::char_map[i].c) {
			return i;
		}
	}
	return 0;
}

void disp_stm32_t::flush() {
	while (dirty_queue.size() > 0) {
		const auto& area = dirty_queue.peek();
		_draw_dirty(area);
		dirty_queue.discard();
	}

	while (string_buffer.size() && !dirty_queue.is_full()) {
		auto c = string_buffer.pop();
		auto *area = dirty_queue.emplace(cursor_x, cursor_y, 16, 16);	
		cursor_x += 8;

		area->fill(0x0000);
		auto& char_data = Draw::char_map[font_lookup(c)].data;
		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				bool px = (char_data[i] >> j) & 0x01;
				if (px) {
					area->buffer[8-j][i] = 0xFFFF;
				}
			}
		}
	}
}

}
