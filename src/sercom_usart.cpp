#include <stdio.h>
#include "sam/clock.h"
#include "sam/sercom_usart.h"
#include "sam.h"

uint16_t baud_rate_reg(uint32_t baud, uint32_t ref_clock) {
	// baud = 65536 * (1-S*(fbaud/fref))
	// baud = 65536-65536*S*fbaud/fref
	// S (number of samples) = 16 in our config
	return 65536ul - (65536ul * 16ul * baud) / ref_clock;
}

void uart_init() {
	hack_clock_setup_sercom0();
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	SERCOM0->USART.CTRLA.bit.SWRST = 1;
	while (SERCOM0->USART.SYNCBUSY.bit.SWRST) {};

	SERCOM0->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(SERCOM_USART_CTRLA_MODE_USART_INT_CLK_Val) |
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

	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_CHSIZE(0) |
	SERCOM_USART_CTRLB_PMODE; // 1 = odd parity
	// ctrlb:
	// chsize
	// pmode
	// stop bits

	// sampr = 0 => 16x, arithmetic

	// set baud
	SERCOM0->USART.BAUD.reg = 50435; // hardcoded for 115200

	SERCOM0->USART.CTRLB.reg |= SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN;
	// No sync if uart is disabled

	// enable in ctrla

	SERCOM0->USART.CTRLA.bit.ENABLE = 1;
	while (SERCOM0->USART.SYNCBUSY.bit.ENABLE) {};
}

void uart_putchar(char c) {
	while(!SERCOM0->USART.INTFLAG.bit.DRE) {};
	SERCOM0->USART.DATA.reg = c;
}

void uart_puts(const char *c) {
	while(*c) {
		uart_putchar(*c);
		c++;
	}
}