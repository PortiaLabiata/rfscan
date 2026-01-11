#include "spi.hpp"
#include "stm32f103xb.h"
#include "halconf.h"
#include "osal.hpp"

namespace HAL {

void spidrv_t::init() {
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
#if SPI_1 == ON
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
#endif
#if SPI_2 == ON
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
#endif
}

static const uint32_t spi_br_table[] = {
	2, 4, 8, 16, 32, 64, 128, 256,
};

static inline void _spi_setmode(SPI_TypeDef *spi, spi_mode_t mode) {
	spi->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	switch (mode) {
		case spi_mode_t::mode_0:
			break;
		case spi_mode_t::mode_1:
			spi->CR1 |= SPI_CR1_CPHA;
			break;
		case spi_mode_t::mode_2:
			spi->CR1 |= SPI_CR1_CPOL;
			break;
		case spi_mode_t::mode_3:
			spi->CR1 |= SPI_CR1_CPOL;
			spi->CR1 |= SPI_CR1_CPHA;
			break;
		default:
			break;
	}
}

status_t spidrv_t::cfg(uint32_t freq) {
	osal->assert(spi == SPI1 || spi == SPI2, "spi invalid driver");

	spi->CR1 |= cr1;
	spi->CR2 |= cr2;

	auto index = 0;
	if (spi == SPI1) {
		index = (RCC->CFGR & RCC_CFGR_PPRE2) >> 11U;
	} else if (spi == SPI2) {
		index = (RCC->CFGR & RCC_CFGR_PPRE1) >> 8U;
	}

	const auto ppre = HAL::apb_presc_table[index];
	osal->assert(ppre != 0, "spi div0");

	const auto pclk = SystemCoreClock / ppre;	
	osal->assert(freq * 2 == pclk || freq * 4 == pclk ||
				freq * 8 == pclk || freq * 16 == pclk ||
				freq * 32 == pclk || freq * 64 == pclk ||
				freq * 128 == pclk || freq * 256 == pclk,
				"spi invalid freq");

	auto speed = pclk / freq;
	for (int i = 0; i < 8; i++) {
		if (speed == spi_br_table[i]) {
			speed = i;
			break;
		}
	}

	spi->CR1 |= speed << SPI_CR1_BR_Pos; 
	_spi_setmode(spi, mode);

	spi->CR1 |= SPI_CR1_MSTR;
	spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	spi->CR1 |= SPI_CR1_SPE;

	return status_t::ok;
}

void spidrv_t::exchange(uint8_t *txbuf, uint8_t *rxbuf,
				size_t size) {
	osal->assert(txbuf != NULL, "null buf");

	for (size_t i = 0; i < size; i++) {
		if (rxbuf && spi->SR & SPI_SR_RXNE) {
			rxbuf[i] = spi->DR;
		}
		while (!(spi->SR & SPI_SR_TXE)) ;
		spi->DR = txbuf[i];
	}
}

}
