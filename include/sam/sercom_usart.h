#pragma once
#include <stdint.h>
#include <stdbool.h>

void uart_init();
void uart_putchar(char c);
void uart_puts(const char *c);