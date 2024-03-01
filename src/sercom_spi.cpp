#include <stdio.h>
#include "sam/clock.h"
#include "sam/sercom_spi.h"
#include "sam/gpio.h"
#include "sam.h"

static uint16_t baud_rate_reg_sync(uint32_t baud, uint32_t ref_clock) {
	// baud = 65536 * (1-S*(fbaud/fref))
	// baud = 65536-65536*S*fbaud/fref
	// S (number of samples) = 16 in our config
	return ref_clock / (2*baud) - 1;
}

template<int N> void SercomSPI<N>::init(unsigned int dipo, unsigned int dopo) {
	hack_clock_setup_sercom0();
	hack_clock_setup_sercom1();
	hack_clock_setup_sercom2();
	hack_clock_setup_sercom3();

	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM1;
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM2;
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM3;

	sercom_ptr->SPI.CTRLA.bit.SWRST = 1;
	while (sercom_ptr->SPI.SYNCBUSY.bit.SWRST) {};

	// TODO: configurable mode (cpol, cpha bits)
	// Form 0 = SPI
	// DIPO
	// DOPO

	sercom_ptr->USART.CTRLA.reg = /*SERCOM_SPI_CTRLA_DORD |*/
	/*SERCOM_SPI_CTRLA_CPOL |*/
	/*SERCOM_SPI_CTRLA_CPHA |*/
	SERCOM_SPI_CTRLA_FORM(0) |
	SERCOM_SPI_CTRLA_DIPO(dipo) |
	SERCOM_SPI_CTRLA_DOPO(dopo) |
	SERCOM_SPI_CTRLA_MODE_SPI_MASTER;

	// TODO: configurable spi CS control, spi size??
	// 8 bit size, turn on.
	// Don't use hardware CS.

	sercom_ptr->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(0);

	// set baud
	sercom_ptr->SPI.BAUD.reg = baud_rate_reg_sync(100000, 8000000);

	sercom_ptr->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
	// No sync if SPI is disabled

	// Finally, enable in CTRLA
	sercom_ptr->USART.CTRLA.bit.ENABLE = 1;
	while (sercom_ptr->USART.SYNCBUSY.bit.ENABLE) {};
}

template<int N> void SercomSPI<N>::send_byte(uint8_t c) {
	while(!sercom_ptr->SPI.INTFLAG.bit.DRE) {};
	sercom_ptr->SPI.DATA.reg = c;
}

template<int N> void SercomSPI<N>::transfer(uint8_t *data, size_t length) {
	port_set_value(PORT_A, 16, false);
	for(size_t i = 0; i < length; i++) {
		send_byte(data[i]);
	}

	// Wait for end of transfer before releasing CS.
	while(!sercom_ptr->SPI.INTFLAG.bit.TXC) {};
	port_set_value(PORT_A, 16, true);
}

template class SercomSPI<0>;
template class SercomSPI<1>;
template class SercomSPI<2>;
template class SercomSPI<3>;
