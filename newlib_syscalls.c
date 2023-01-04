#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sam/sercom_usart.h"

__attribute__((used)) void _init() {
}

char fake_heap[0x800];
static char *heap_start = fake_heap;
static char *heap_end = NULL;

__attribute__((used)) void* _sbrk(int amount) {
	char *prev_heap_end;

	if(heap_end == NULL) {
		heap_end = heap_start;
	}
	prev_heap_end = heap_end;
	heap_end += amount;

	if((heap_end - heap_start) > sizeof(fake_heap)) {
		// die
		uart_puts("out of fixed heap\n");
		abort();
	}

	return prev_heap_end;
}

__attribute__((used)) int _write (int fd, char *buf, int count) {
	int written = 0;

	(void)fd;

	for (; count != 0; --count) {
		uart_putchar((uint8_t)*buf++);
		++written;
	}
	return written;
}

__attribute__((used)) int _close() {
	uart_puts("_close stub\r\n");
	abort();
}

#include <sys/stat.h>
__attribute__((used)) int _fstat (int fd, struct stat *sbuf) {
	if(fd == 0 || fd == 1 || fd == 2) {
		sbuf->st_mode = S_IFCHR;
	} else {
		uart_puts("_fstat stub\r\n");
		abort();
	}
	return 0;
}

__attribute__((used)) int _isatty(int fd) {
	if(fd == 0 || fd == 1 || fd == 2) {
		return 1;
	} else {
		return 0;
	}
}

__attribute__((used)) int _lseek() {
	uart_puts("_lseek stub\r\n");
	abort();
}

__attribute__((used)) int _read() {
	uart_puts("_read stub\r\n");
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
		//uart_puts("in _exit\n");
	}
}