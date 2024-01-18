#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sam/sercom_usart.h"

__attribute__((used)) void _init() {
}

char fake_heap[1];
static char *heap_start = fake_heap;
static char *heap_end = NULL;

__attribute__((used)) void* _sbrk(ptrdiff_t incr) {
	char *prev_heap_end;

	if(heap_end == NULL) {
		heap_end = heap_start;
	}
	prev_heap_end = heap_end;
	heap_end += incr;

	if((unsigned int)(heap_end - heap_start) > sizeof(fake_heap)) {
		// die
		uart_puts("out of fixed heap\n");
		abort();
	}

	return prev_heap_end;
}

__attribute__((used)) int _write(int fd, const void *buf, size_t nbytes) {
	(void)fd;

	const uint8_t *buf_char = (const uint8_t*)buf;

	for (size_t i = 0; i < nbytes; i++) {
		uart_putchar(buf_char[i]);
	}
	return nbytes;
}

__attribute__((used)) int _close(int fd) {
	(void)fd;

	uart_puts("_close stub\r\n");
	abort();
}

#include <sys/stat.h>
__attribute__((used)) int _fstat(int fd, struct stat *sbuf) {
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

__attribute__((used)) off_t _lseek(int fd, off_t offset, int whence) {
	(void)fd;
	(void)offset;
	(void)whence;

	uart_puts("_lseek stub\r\n");
	abort();
}

__attribute__((used)) int _read(int fd, void *buf, size_t nbytes) {
	(void)fd;
	(void)buf;
	(void)nbytes;

	uart_puts("_read stub\r\n");
	abort();
}

__attribute__((used)) int _kill(pid_t pid, int sig) {
	(void)pid;
	(void)sig;

	return 0;
}

__attribute__((used)) pid_t _getpid() {
	return 0;
}

__attribute__((used)) void _exit() {
	uart_puts("in _exit\n");
	while(true) {}
}

// Add a definition of __assert_func that doesn't print.
void __assert_func (const char *file,
        int line,
        const char *func,
        const char *failedexpr)
{
	(void)file;
	(void)line;
	(void)func;
	(void)failedexpr;

	abort();
}