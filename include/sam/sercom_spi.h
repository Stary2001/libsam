#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"
#include "_internal_get_sercom.h"

template<int N> class SercomSPI {
public:
	void init(unsigned int dipo, unsigned int dopo);

	Sercom *const sercom_ptr = get_sercom_ptr<N>();

	//void read();
	//void write();
	void transfer(uint8_t *data, size_t length);
private:
	void send_byte(uint8_t c);
};

void spi_init();
void spi_transfer();
