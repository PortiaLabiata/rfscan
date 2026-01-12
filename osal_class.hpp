#pragma once
#include "common.hpp"

namespace OSAL {

class disp_backend_t {
public:
	static const size_t _sizex = 240;
	static const size_t _sizey = 240;
	static const size_t _dirty_maxx = 16;
	static const size_t _dirty_maxy = 16;
	static const size_t _stack_size = 8;

	disp_backend_t() = default;
	virtual ~disp_backend_t() {};

	virtual void init() = 0;
	virtual void flush() = 0;
	virtual void clear() = 0;

	using pixel_t = uint16_t;
	using row_t = array_t<pixel_t, _dirty_maxx>;
	struct dirty_t {
		size_t x, y;
		size_t size_x, size_y;
		pixel_t buffer[_dirty_maxx][_dirty_maxy];

		dirty_t() = default;
		dirty_t(size_t _x, size_t _y, size_t _size_x,
						size_t _size_y) :
			x(_x), y(_y), size_x(_size_x), size_y(_size_y) {}

		void fill(pixel_t px) {
			size_t i, j;
			for (i = 0; i < size_x; i++) {
				for (j = 0; j < size_y; j++) {
					buffer[i][j] = px;
				}
			}
		}

		void set_pixel(size_t x, size_t y, pixel_t px) {
			buffer[x][y] = px;
		}

		static pixel_t encode_color(uint8_t r, 
						uint8_t g, uint8_t b) {
			r = static_cast<uint16_t>(r)*32/256;
			g = static_cast<uint16_t>(g)*64/256;
			b = static_cast<uint16_t>(b)*32/256;

			return ((r & 0b11111) << 11)  | 
					((g & 0b111111) << 5) |
					(b & 0b11111);
		}

		static void decode_color(pixel_t px, uint8_t& r,
						uint8_t& g, uint8_t& b) {
			r = px >> 11;
			g = (px >> 5) & 0b111111;
			b = px & 0b11111;

			r = static_cast<uint16_t>(r)*256/32;
			g = static_cast<uint16_t>(g)*256/64;
			b = static_cast<uint16_t>(b)*256/32;
		}
	};

	array_t<dirty_t, _stack_size> dirty_stack;
};
using dirty_t = disp_backend_t::dirty_t;

class radio_backend_t {
public:
	radio_backend_t() = default;
	virtual ~radio_backend_t() {};
	static const size_t buf_size = 256;

	virtual array_t<rssi_t, buf_size> *rssi_scan(freq_t start, 
					freq_t stop, freq_t step) = 0;

	virtual void set_freq(freq_t f) = 0;
	virtual void set_band(freq_t b) = 0;
};

class osal_backend_t {
public:
	osal_backend_t() = default;
	virtual ~osal_backend_t() {};

	virtual void init() = 0;
	virtual systime_t millis() = 0;
	virtual void delay(systime_t ms) = 0;
	virtual void assert(bool val, const char *reason) = 0;
};

}
