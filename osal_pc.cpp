#include "osal_pc.hpp"
#include <iostream>
#include <cstdlib>

namespace OSAL {

void osal_pc_t::init() {
	assert(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) 
					== 0, "init"); 
	_start_ms = SDL_GetTicks();	
}

systime_t osal_pc_t::millis() {
	return SDL_GetTicks() - _start_ms;
}

void osal_pc_t::delay(systime_t ms) {
	SDL_Delay(ms);
}

void osal_pc_t::assert(bool v, const char *reason) {
	if (!v) {
		std::cout << "Static assertion triggered: " <<
				reason << std::endl;
		std::exit(-1);
	}
}

}
