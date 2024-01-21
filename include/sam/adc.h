#pragma once
#include <stdint.h>
#include <stdbool.h>

class ADCClass {
public:
	void init();
	void select(unsigned int n);
	uint16_t read();
};