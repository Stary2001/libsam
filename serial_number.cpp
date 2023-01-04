extern "C" {
	#include "siphash.h"
}
#include "sam/serial_number.h"

static uint32_t serial_storage[4] = {0};
uint32_t *serial_get_raw() {
	if(serial_storage[0] == 0) {
		serial_storage[0] = *(uint32_t*)0x0080A00C;
		serial_storage[1] = *(uint32_t*)0x0080A040;
		serial_storage[2] = *(uint32_t*)0x0080A044;
		serial_storage[3] = *(uint32_t*)0x0080A048;
	}
	return serial_storage;
}

char *hex = "0123456789abcdef";

static uint8_t serial_hash[17];
uint8_t *serial_get_hash_hex() {
	uint32_t *serial = serial_get_raw();

	uint8_t hash_out[8];
	siphash(serial, 16, serial, hash_out, 8);

	for(int i = 0; i < 8; i++) {
		serial_hash[i*2] = hex[hash_out[i] & 0xf];
		serial_hash[i*2 + 1] = hex[(hash_out[i] & 0xf0) >> 4];
	}
	serial_hash[16] = 0;
	return serial_hash;
}