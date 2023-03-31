#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"

#ifdef __cplusplus

template<int N> constexpr Sercom * get_sercom_ptr() {
	static_assert(N < 3);

	if constexpr(N == 0) {
		return SERCOM0;
	} else if(N == 1) {
		return SERCOM1;
	} else if(N == 2) {
		return SERCOM2;
	}
}

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

extern "C" {
#endif

void spi_init();
void spi_transfer();

#ifdef __cplusplus
}
#endif