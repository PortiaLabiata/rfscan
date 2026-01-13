#include "disp_pc.hpp"
#include "osal.hpp"
#include "font.hpp"

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
			const SDL_Rect rect = {static_cast<int>(j+area.x)*ZOOM, 
					static_cast<int>(i+area.y)*ZOOM, ZOOM, ZOOM};

			SDL_RenderFillRect(renderer, &rect);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

size_t font_lookup(char c) {
	for (size_t i = 0; i < sizeof(Draw::char_map)/sizeof(Draw::char_t); 
					i++) {
		if (c == Draw::char_map[i].c) {
			return i;
		}
	}
	return 0;
}

void disp_pc_t::flush() {
	SDL_Event evt;
	if (SDL_PollEvent(&evt) != 0) {
		if (evt.type == SDL_QUIT) {
			deinit_flag = true;
		}
	}
	bool have_updates = false;
	if (dirty_queue.size()) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		have_updates = true;
	}

	while (dirty_queue.size() > 0) {
		draw_area(dirty_queue.pop());
	}

	while (string_buffer.size() && !dirty_queue.is_full()) {
		auto c = string_buffer.pop();
		auto *area = dirty_queue.emplace(cursor_x, cursor_y, 8, 8);	
		cursor_x += 8;

		area->fill(0x0000);
		auto& char_data = Draw::char_map[font_lookup(c)].data;
		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				bool px = (char_data[i] >> j) & 0x01;
				if (px) {
					area->buffer[i][8-j] = 0xFFFF;
				}
			}
		}
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
