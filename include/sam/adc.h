#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
class ADCClass {
public:
	void init();
	void select(int n);
	uint16_t read();
private:
	void send_byte(uint8_t c);
};

extern "C" {
#endif

#ifdef __cplusplus
}
#endif