#pragma once

namespace OS {

class mutex_abstract_t {
public:
	mutex_abstract_t() = default;
	virtual ~mutex_abstract_t() {};

	virtual void lock() = 0;
	virtual void unlock() = 0;

	template <typename T>
	T create() {
		return T();
	}
};

class lock_guard_t {
public:
	lock_guard_t(mutex_abstract_t& mtx) : _mtx(mtx) {
		mtx.lock();
	};

	~lock_guard_t() {
		_mtx.unlock();
	}
private:
	mutex_abstract_t& _mtx;
};

}
