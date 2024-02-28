#pragma once
#include <stdint.h>
#include <stdbool.h>

class ADCClass {
public:
	void init(int average, int samplen);
	void select(unsigned int n);
	uint16_t read();
	void read_with_input_scan(uint16_t *values, unsigned int num);
};