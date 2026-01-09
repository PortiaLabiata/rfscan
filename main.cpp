#include "common.hpp"
#include "gpio.hpp"
#include "spi.hpp"

using namespace HAL;

spidrv_t spi{
	.spi = SPI1,
	.mode = spi_mode_t::mode_3,
};

ioline_t led_line{
	.port = GPIOC,
	.pin = 13UL
};

inline void spi_init() {
	ioline_t miso_line{GPIOA, 6UL};
	ioline_t mosi_line{GPIOA, 7UL};
	ioline_t sck_line{GPIOA,  5UL};

	miso_line.cfg(linemode_t::in_float);
	mosi_line.cfg(linemode_t::af_pp);
	sck_line.cfg(linemode_t::af_pp);

	spi.cfg(31250);
}

int main(void) {
	sys::init();
	ioline_t::init();
	spidrv_t::init();

	led_line.cfg(linemode_t::out_pp);
	spi_init();	

	while (1) {
		led_line.toggle();
		spi.write(0xDE);
		sys::delay(500);
	}
}
