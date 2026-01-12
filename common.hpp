#pragma once
#include <stddef.h>
#include <stdint.h>
#include "mutex.hpp"

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
		_size(0), _cap(cap_) {
	};

	T *data() { return a; }
	size_t size() {
		OS::lock_guard_t guard(sem);
		return _size; 
	}
	size_t cap() const { return _cap; }

	void push(const T& v) {
		OS::lock_guard_t guard(sem);
		if (_size < _cap) {
			a[_size++] = v;
		}
	}

	void emplace(const T&& v) {
		OS::lock_guard_t guard(sem);
		if (_size < _cap) {
			a[_size++] = v;
		}
	}

	T pop() { 
		OS::lock_guard_t guard(sem);
		if (_size) _size--;
		return a[_size]; 
	}
	T& last() {
		OS::lock_guard_t guard(sem);
		return a[_size-1];
	}

	void reset() { _size = 0; }

	void copy(array_t<T, cap_>& other) {
		memcpy(other.a, a, _size);
	}

private:
	T a[cap_];
	size_t _size, _cap;
	OS::mutex_t sem;
};

namespace HAL {

extern size_t apb_presc_table[8U];

}
