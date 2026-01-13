#include <stdarg.h>
#include <string.h>
#include "osal_class.hpp"
#include "env.hpp"

namespace OSAL {

void dirty_t::fill(pixel_t px) {
	size_t i, j;
	for (i = 0; i < size_x; i++) {
		for (j = 0; j < size_y; j++) {
			buffer[i][j] = px;
		}
	}
}

pixel_t dirty_t::encode_color(uint8_t r, uint8_t g, 
				uint8_t b) {
	r = static_cast<uint16_t>(r)*32/256;
	g = static_cast<uint16_t>(g)*64/256;
	b = static_cast<uint16_t>(b)*32/256;

	return ((r & 0b11111) << 11)  | 
			((g & 0b111111) << 5) |
			(b & 0b11111);
}

void dirty_t::decode_color(pixel_t px, uint8_t& r,
						uint8_t& g, uint8_t& b) {
	r = px >> 11;
	g = (px >> 5) & 0b111111;
	b = px & 0b11111;

	r = static_cast<uint16_t>(r)*256/32;
	g = static_cast<uint16_t>(g)*256/64;
	b = static_cast<uint16_t>(b)*256/32;
}

#if !SITL
int abs(int x) { return x >= 0 ? x : -x; }
#endif

size_t disp_backend_t::printf(size_t x, size_t y, 
				const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	cursor_x = x;
	cursor_y = y;
	
	size_t i = 0;
	const auto max = static_cast<size_t>(MIN(_buffer_size, 
							strlen(fmt)));
	for (; i < max; i++) {
		if (fmt[i] != '%') {
			string_buffer.push(fmt[i]);
			continue;
		}

		switch (fmt[++i]) {
			case 's': {
				char *s = va_arg(args, char*);
				for (size_t j = 0; j < strlen(s); j++) {
					string_buffer.push(s[j]);
				}
				break;
			}
			case 'd': {
				int d = abs(va_arg(args, int));
				while (d > 0) {
					string_buffer.push('0' + (d % 10));
					d /= 10;
				}
				break;
			}
			default:
				break;
		}
	}

	va_end(args);
	return i;
}

}
