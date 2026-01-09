#pragma once
#include "common.hpp"
#include "env.hpp"

namespace OSAL {

class disp_backend_t {
protected:
	static const size_t _sizex = 240;
	static const size_t _sizey = 240;
	static const size_t _dirty_maxx = 16;
	static const size_t _dirty_maxy = 16;
	static const size_t _stack_size = 16;
public:
	disp_backend_t() = default;
	virtual ~disp_backend_t() {};

	virtual void init() = 0;
	virtual void flush() = 0;

	using pixel_t = uint16_t;
	using row_t = array_t<pixel_t, _dirty_maxx>;
	struct dirty_t {
		array_t<row_t, _dirty_maxy> area;
		size_t x, y;

		size_t size_x() const { return area.data()->size(); }
		size_t size_y() const { return area.size(); }
	};

	array_t<dirty_t, _stack_size> dirty_stack;
};

class radio_backend_t {
public:
	radio_backend_t() = default;
	virtual ~radio_backend_t() {};
	static const size_t buf_size = 256;

	virtual array_t<rssi_t, buf_size> rssi_scan(freq_t start, 
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
