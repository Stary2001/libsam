#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sam/sercom_usart.h"

__attribute__((used)) void _init() {
}

__attribute__((used)) void _sbrk() {
	// No malloc!
}

__attribute__((used)) int _write (int fd, char *buf, int count) {
	int written = 0;

	(void)fd;

	for (; count != 0; --count) {
		/*if (uart_putchar((uint8_t)*buf++)) {
			return -1;
		}*/
		uart_putchar((uint8_t)*buf++);
		++written;
	}
	return written;
}

__attribute__((used)) int _close() {
	abort();
}

__attribute__((used)) int _fstat() {
	abort();
}

__attribute__((used)) int _isatty() {
	abort();
}

__attribute__((used)) int _lseek() {
	abort();
}

__attribute__((used)) int _read() {
	abort();
}

__attribute__((used)) int _kill() {
	return 0;
}

__attribute__((used)) int _getpid() {
	return 0;
}

__attribute__((used)) int _exit() {
	while(true) {
		//
	}
}