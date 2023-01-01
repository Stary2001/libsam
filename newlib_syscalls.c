#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

int uart_putchar(uint8_t c) {
	// trust me
	(void) c;
	return 0;
}

void _init() {
}

void _sbrk() {
	// No malloc!
}

int _write (int fd, char *buf, int count) {
	int written = 0;

	(void)fd;

	for (; count != 0; --count) {
		if (uart_putchar((uint8_t)*buf++)) {
			return -1;
		}
		++written;
	}
	return written;
}

int _close() {
	abort();
}

int _fstat() {
	abort();
}

int _isatty() {
	abort();
}

int _lseek() {
	abort();
}

int _read() {
	abort();
}

int _kill() {
	return 0;
}

int _getpid() {
	return 0;
}

int _exit() {
	while(true) {
		//
	}
}