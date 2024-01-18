#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
class ADCClass {
public:
	void init();
	void select(unsigned int n);
	uint16_t read();
};

extern "C" {
#endif

#ifdef __cplusplus
}
#endif