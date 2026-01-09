#include "disp_pc.hpp"
#include "osal.hpp"
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_surface.h>

static const auto ZOOM = 2;

namespace OSAL {

void disp_pc_t::init() {
	osal->assert(SDL_Init(SDL_INIT_EVERYTHING) == 0, "init"); 
	window = SDL_CreateWindow("SITL", 
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
					_sizex*ZOOM, _sizey*ZOOM, \
					SDL_WINDOW_ALLOW_HIGHDPI);
	osal->assert(window != NULL, "window");

    renderer = SDL_CreateRenderer(window, -1, 
					SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	osal->assert(renderer != NULL, "renderer");

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	
}

void disp_pc_t::draw_area(const disp_backend_t::dirty_t& area) {
	const auto end_x = area.x + area.size_x();
	const auto end_y = area.y + area.size_y();

	for (auto i = area.x; i < end_x; i++) {
		const auto& row = area.area.data()[i];
		for (auto j = area.y; j < end_y; j++) {
			const auto pixel = row.data()[j]; 

			const auto red = pixel >> 11;
			const auto green = (pixel >> 5) & 0b111111;
			const auto blue = pixel & 0b11111;
			const SDL_Rect rect = {static_cast<int>(i)*ZOOM, 
					static_cast<int>(j)*ZOOM, ZOOM, ZOOM};

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void disp_pc_t::flush() {
	SDL_Event evt;
	if (SDL_PollEvent(&evt) != 0) {
		if (evt.type == SDL_QUIT) {
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
			std::exit(0);
		}
	}

	while (dirty_stack.size() > 0) {
		dirty_t area = dirty_stack.pop();
		draw_area(area);
	}
	SDL_RenderPresent(renderer);
}

}
