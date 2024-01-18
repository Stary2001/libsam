#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"

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

template<int N> class SercomUart {
public:
	void init(unsigned int dipo, unsigned int dopo);

	Sercom *const sercom_ptr = get_sercom_ptr<N>();

	void puts(const char *data);
	void putchar(char c);
};

void uart_init();
void uart_putchar(char c);
void uart_puts(const char *c);