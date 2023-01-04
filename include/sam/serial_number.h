#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t *serial_get_raw();
uint8_t *serial_get_hash_hex();

#ifdef __cplusplus
}
#endif