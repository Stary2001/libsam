#include <stdio.h>
#include "sam/clock.h"
#include "sam/sercom_usart.h"
#include "sam.h"

static uint16_t baud_rate_reg(uint32_t baud, uint32_t ref_clock) {
	// baud = 65536 * (1-S*(fbaud/fref))
	// baud = 65536-65536*S*fbaud/fref
	// S (number of samples) = 16 in our config
	return 65536ul - (16ul * baud) / (ref_clock / 65536);
}

template<int N> void SercomUart<N>::init(unsigned int txpo, unsigned int rxpo) {
	hack_clock_setup_sercom0();
	hack_clock_setup_sercom1();

	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM1;

	sercom_ptr->USART.CTRLA.bit.SWRST = 1;
	while (sercom_ptr->USART.SYNCBUSY.bit.SWRST) {};

	sercom_ptr->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(SERCOM_USART_CTRLA_MODE_USART_INT_CLK_Val) |
	// cmode = 0
	SERCOM_USART_CTRLA_RXPO(1) |
	SERCOM_USART_CTRLA_TXPO(0) |
	SERCOM_USART_CTRLA_DORD |
	SERCOM_USART_CTRLA_FORM(1);
	// sampr = 0

	// ctrla:
	// mode
	// cmode
	// rxpo
	// txpo
	// dord
	// parity

	sercom_ptr->USART.CTRLB.reg = SERCOM_USART_CTRLB_CHSIZE(0) |
	SERCOM_USART_CTRLB_PMODE; // 1 = odd parity
	// ctrlb:
	// chsize
	// pmode
	// stop bits

	// sampr = 0 => 16x, arithmetic

	// set baud
	sercom_ptr->USART.BAUD.reg = baud_rate_reg(115200, 8000000);

	sercom_ptr->USART.CTRLB.reg |= SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN;
	// No sync if uart is disabled

	// enable in ctrla

	sercom_ptr->USART.CTRLA.bit.ENABLE = 1;
	while (sercom_ptr->USART.SYNCBUSY.bit.ENABLE) {};
}

template<int N> void SercomUart<N>::putchar(char c) {
	while(!sercom_ptr->USART.INTFLAG.bit.DRE) {};
	sercom_ptr->USART.DATA.reg = c;
}

template<int N> void SercomUart<N>::puts(const char *s) {
	while(*s) {
		this->putchar(*s);
		s++;
	}
}

void uart_putchar(char c) {
	(void)c;
	// todo
}

void uart_puts(const char *c) {
	(void)c;
	// todo
}


template class SercomUart<0>;
template class SercomUart<1>;
template class SercomUart<2>;