#pragma once
#include <SDL2/SDL.h>
#include <SDL_mutex.h>
#include "mutex_class.hpp"

namespace OS {

class mutex_pc_t : public mutex_abstract_t {
public:
	mutex_pc_t() { mtx = SDL_CreateMutex(); }
	~mutex_pc_t() { SDL_DestroyMutex(mtx); }
	
	void lock() override { SDL_LockMutex(mtx); }
	void unlock() override { SDL_UnlockMutex(mtx); }

private:
	SDL_mutex *mtx;	
};

}
