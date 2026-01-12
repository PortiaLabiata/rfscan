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
	void clear() override;

private:
	void _draw_dirty(const dirty_t& area);

	spidrv_t spi{SPI1, spi_mode_t::mode_3};
	ioline_t rst_line{GPIOA, 3U};
	ioline_t dc_line{GPIOA, 2U};

	inline void hard_reset() {
		rst_line.clear();
		osal->delay(100);
		rst_line.set();
	}

	inline void set_command() { dc_line.clear(); }
	inline void set_data() { dc_line.set(); }

	void send_command(uint8_t *buf, size_t size);
	void send_data(uint8_t *buf, size_t size);
	inline void send_commandb(uint8_t byte) {
		send_command(&byte, 1);
	}
	inline void send_datab(uint8_t byte) {
		send_data(&byte, 1);
	}

	void set_mode(uint8_t cmd, uint16_t start, uint16_t end) {
		uint8_t _buf[] = { 
			static_cast<uint8_t>(start >> 8), 
			static_cast<uint8_t>(start & 0xFF), 
			static_cast<uint8_t>(end >> 8), 
			static_cast<uint8_t>(end & 0xFF), 
		}; 
		send_commandb(cmd);
		send_data(_buf, sizeof(_buf));
	}

	inline void set_column_mode(uint16_t start, uint16_t end) {
		set_mode(0x2A, start, end);
	}

	inline void set_row_mode(uint16_t start, uint16_t end) {
		set_mode(0x2B, start, end);
	}
};

}
