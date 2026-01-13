#pragma once
#include <stddef.h>
#include <stdint.h>
#include "mutex.hpp"
#include "mutex_class.hpp"

enum class status {
	ok,
	err_arg,
};
using systime_t = uint32_t;
using status_t = status;
using freq_t = uint32_t;
using rssi_t = uint32_t;

inline void *operator new(size_t size, void *ptr) noexcept {
	(void)size;
	return ptr;
}
inline void operator delete(void*, void*) {}

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

	template <typename... Args>
	void emplace(Args&& ...args) {
		OS::lock_guard_t guard(sem);
		if (_size < _cap) {
			new (&a[_size]) T(args...);
			_size++;
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

template <typename T, size_t cap_>
class ring_buffer_t : public OS::mutex_abstract_t {
public:
	ring_buffer_t() = default;
	~ring_buffer_t() = default;

	// Will fail silently in case of 
	// overflow
	void push(const T& v) {
		OS::lock_guard_t guard(mtx);
		if (_size < _cap) {
			a[head] = v;
			head = (head + 1) % _cap;
			_size++;
		}
	}

	// Meant to be used inside lock guard
	// block
	template <typename... Args>
	T *emplace(Args&& ...args) {
		if (_size < _cap) {
			new (&a[head]) T(args...);
			auto* res = &a[head];

			head = (head + 1) % _cap;
			_size++;
			return res;
		}
		return nullptr;
	}

	T pop() {
		OS::lock_guard_t guard(mtx);
		auto tail_old = tail;
		tail = (tail + 1) % _cap;
		_size--;
		return a[tail_old];
	}

	T& peek() {
		OS::lock_guard_t guard(mtx);
		return a[tail];
	}

	void discard() { 
		OS::lock_guard_t guard(mtx);
		if (_size) {
			tail = (tail + 1) % _cap; 
			_size--;
		}
	}

	size_t size() { return _size; }
	size_t cap() { return _cap; }
	bool is_full() { return _size >= _cap; }
	T& get_head() { return a[head]; }

	// Duct tape
	void lock() override { mtx.lock(); }
	void unlock() override { mtx.unlock(); }

private:
	OS::mutex_t mtx;

	size_t _size = 0;
	size_t _cap = cap_;

	T a[cap_];
	size_t head, tail;
};

inline int MIN(int a, int b) {
	return a < b ? a : b;
}

inline int MAX(int a, int b) {
	return a < b ? b : a;
}

namespace HAL {

extern size_t apb_presc_table[8U];

}
