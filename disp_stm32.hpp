#pragma once
#include "osal.hpp"
#include "spi.hpp"
#include "gpio.hpp"

using namespace HAL;
namespace OSAL {

class disp_stm32_t : public disp_backend_t {
public:
	disp_stm32_t() : disp_backend_t() {};
	~disp_stm32_t() {};

	void init() override;
	void flush() override;

private:
	spidrv_t spi{SPI1, spi_mode_t::mode_3};
	ioline_t rst_line{GPIOA, 2U};
	ioline_t dc_line{GPIOA, 1U};

	inline void hard_reset() {
		rst_line.clear();
		osal->delay(100);
		rst_line.set();
	}

	inline void set_command() { dc_line.set(); }
	inline void set_data() { dc_line.clear(); }

	void send_command(uint8_t *buf, size_t size);
	void send_data(uint8_t *buf, size_t size);
	inline void send_commandb(uint8_t byte) {
		send_command(&byte, 1);
	}
};

}
