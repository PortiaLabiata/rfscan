#pragma once
#include "common.hpp"

namespace OSAL {

using pixel_t = uint16_t;
class disp_backend_t {
public:
	static const size_t _sizex = 240;
	static const size_t _sizey = 240;
	static const size_t _dirty_maxx = 16;
	static const size_t _dirty_maxy = 16;
	static const size_t _queue_size = 8;
	static const size_t _buffer_size = 32;

	disp_backend_t() = default;
	virtual ~disp_backend_t() {};

	virtual void init() = 0;
	virtual void flush() = 0;
	virtual void clear() = 0;

	size_t printf(size_t x, size_t y, const char *fmt, ...);

	using row_t = array_t<pixel_t, _dirty_maxx>;
	struct dirty_t {
		size_t x, y;
		size_t size_x, size_y;
		pixel_t buffer[_dirty_maxx][_dirty_maxy];

		dirty_t() = default;
		dirty_t(size_t _x, size_t _y, size_t _size_x,
						size_t _size_y) :
			x(_x), y(_y), size_x(_size_x), size_y(_size_y) {}

		void fill(pixel_t px);

		void set_pixel(size_t x, size_t y, pixel_t px) {
			buffer[x][y] = px;
		}

		static pixel_t encode_color(uint8_t r, 
						uint8_t g, uint8_t b);

		static void decode_color(pixel_t px, uint8_t& r,
						uint8_t& g, uint8_t& b);
	};

	using dirty_queue_t = ring_buffer_t<dirty_t, _queue_size>;
	dirty_queue_t dirty_queue;
protected:
	ring_buffer_t<char, _buffer_size> string_buffer;
	size_t cursor_x, cursor_y;
};
using dirty_t = disp_backend_t::dirty_t;

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
