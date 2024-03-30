extern "C" {
	#include "halfsiphash.h"
}
#include "sam/serial_number.h"

/*
	10.3.3 Serial Number
	Each device has a unique 128-bit serial number which is a concatenation of four 32-bit words contained at the
	following addresses:
	Word 0: 0x0080A00C
	Word 1: 0x0080A040
	Word 2: 0x0080A044
	Word 3: 0x0080A048
	The uniqueness of the serial number is guaranteed only when using all 128 bits.
*/

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

void serial_get_hash(uint8_t *hash_out) {
	uint8_t *serial = (uint8_t*)serial_get_raw();
	halfsiphash(serial, 16, serial, hash_out, 8);
}

const char *hex = "0123456789abcdef";

static uint8_t serial_hash[17];
uint8_t *serial_get_hash_hex() {
	uint8_t *serial = (uint8_t*)serial_get_raw();

	uint8_t hash_out[8];
	halfsiphash(serial, 16, serial, hash_out, 8);

	for(int i = 0; i < 8; i++) {
		serial_hash[i*2] = hex[hash_out[i] & 0xf];
		serial_hash[i*2 + 1] = hex[(hash_out[i] & 0xf0) >> 4];
	}
	serial_hash[16] = 0;
	return serial_hash;
}