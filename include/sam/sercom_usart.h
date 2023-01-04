#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_init();
void uart_putchar(char c);
void uart_puts(const char *c);

#ifdef __cplusplus
}
#endif