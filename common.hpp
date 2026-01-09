#pragma once
#include <stddef.h>
#include <stdint.h>

enum class status {
	ok,
	err_arg,
};
using systime_t = uint32_t;
using status_t = status;
using freq_t = uint32_t;
using rssi_t = uint32_t;

template <typename T, size_t cap_>
class array_t {
public:
	array_t() :
		a(nullptr), _size(0), _cap(cap_) {};

	T *data() const { return a; }
	size_t size() const { return _size; }
	size_t cap() const { return _cap; }

	void push(T v) {
		if (_size < _cap) {
			a[_size++] = v;
		}
	}

	T pop() { 
		if (_size) _size--;
		return a[_size]; 
	}
	void reset() { _size = 0; }

	void copy(array_t<T, cap_>& other) {
		memcpy(other.a, a, _size);
	}

private:
	T *a;
	size_t _size, _cap;
};

namespace HAL {

extern size_t apb_presc_table[8U];

}
