#pragma once
#include <SDL2/SDL.h>
#include "osal_class.hpp"

namespace OSAL {

class disp_pc_t : public disp_backend_t {
public:
	disp_pc_t() : disp_backend_t() {};
	~disp_pc_t() {};

	void init() override;
	void flush() override;
	void clear() override;

	void deinit();
	bool deinit_flag = false;

private:
	void draw_area(const dirty_t& area);
	SDL_Window *window;
	SDL_Renderer *renderer;
};

}
