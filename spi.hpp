#pragma once
#include "common.hpp"
#include <stddef.h>
#include "stm32f1xx.h"

namespace HAL {

enum class spi_mode_t {
	mode_0,
	mode_1,
	mode_2,
	mode_3,
};

struct spidrv_t {
	SPI_TypeDef *spi;
	spi_mode_t mode;
	uint32_t cr1;
	uint32_t cr2;

	static void init();
	status_t cfg(uint32_t freq);

	void exchange(uint8_t *txbuf, uint8_t *rxbuf, 
					size_t size);
	inline void write(uint8_t byte) {
		exchange(&byte, NULL, 1);
	}
	inline uint8_t read() {
		uint8_t rx, tx = 0;
		exchange(&tx, &rx, 1);
		return rx;
	}
};

}
