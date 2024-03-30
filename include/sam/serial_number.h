#pragma once
#include <stdint.h>
#include <stdbool.h>

uint32_t *serial_get_raw();
void serial_get_hash(uint8_t *out);
uint8_t *serial_get_hash_hex();
