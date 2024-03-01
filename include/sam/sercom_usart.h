#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"
#include "_internal_get_sercom.h"

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