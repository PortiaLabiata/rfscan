#include "gpio.hpp"
#include "halconf.h"

namespace HAL {

void ioline_t::init(void) {
#if GPIO_PORTA == ON
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
#endif
#if GPIO_PORTB == ON
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
#endif
#if GPIO_PORTC == ON
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
#endif
#if GPIO_PORTD == ON
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
#endif
#if GPIO_PORTE == ON
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
#endif
}

status_t ioline_t::cfg(linemode_t mode) {
	auto *cr = &port->CRL;
	auto pos = (pin % 8)*4;
	if (pin >= 8) {
		cr = &port->CRH;	
	}
	*cr &= ~(0b1111 << pos);
	*cr |= (static_cast<uint32_t>(mode) << pos);

	port->ODR &= ~(0x01 << pin);
	if (mode == linemode_t::in_pup) {
		port->ODR |= (0x01 << pin);	
	}
	return status_t::ok;
}

void ioline_t::set() {
	port->ODR |= (0x01 << pin);
}

void ioline_t::clear() {
	port->ODR &= ~(0x01 << pin);
}

void ioline_t::toggle() {
	port->ODR ^= (0x01 << pin);
}

}
