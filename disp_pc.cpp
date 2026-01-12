#include "disp_pc.hpp"
#include "osal.hpp"
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>

static const auto ZOOM = 2;

namespace OSAL {

void disp_pc_t::init() {
	window = SDL_CreateWindow("SITL", 
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
					_sizex*ZOOM, _sizey*ZOOM, \
					SDL_WINDOW_ALLOW_HIGHDPI);
	osal->assert(window != NULL, "window");

    renderer = SDL_CreateRenderer(window, -1, 
					SDL_RENDERER_SOFTWARE);
	osal->assert(renderer != NULL, "renderer");

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	
}

void disp_pc_t::draw_area(const disp_backend_t::dirty_t& area) {
	osal->assert(area.size_x <= _dirty_maxx &&
					area.size_y <= _dirty_maxy, "inv dirty");

	osal->assert(area.x >= 0 && area.y >= 0, "inv dirty");

	const auto end_x = area.x + area.size_x;
	const auto end_y = area.y + area.size_y;

	for (auto i = 0; i < area.size_x; i++) {
		for (auto j = 0; j < area.size_y; j++) {
			const auto pixel = area.buffer[i][j];

			uint8_t red, green, blue;
			dirty_t::decode_color(pixel, red, green, blue);

			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			const SDL_Rect rect = {static_cast<int>(i+area.x)*ZOOM, 
					static_cast<int>(j+area.y)*ZOOM, ZOOM, ZOOM};

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void disp_pc_t::flush() {
	SDL_Event evt;
	if (SDL_PollEvent(&evt) != 0) {
		if (evt.type == SDL_QUIT) {
			deinit_flag = true;
		}
	}
	bool have_updates = false;
	if (dirty_stack.size()) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		have_updates = true;
	}

	while (dirty_stack.size() > 0) {
		const auto area = dirty_stack.pop();
		draw_area(area);
	}
	if (have_updates)
		SDL_RenderPresent(renderer);
}

void disp_pc_t::deinit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::exit(0);
}

void disp_pc_t::clear() {
	SDL_RenderClear(renderer);
}

}
