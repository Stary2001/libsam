#include "sam/gpio.h"
#include "sam.h"

// 0 = input
// 1 = output
void port_set_direction(int n, bool value) {
	if(value) {
		PORT->Group[0].DIRSET.reg = 1 << n;
	} else {
		PORT->Group[0].DIRCLR.reg = 1 << n;
	}
}

void port_set_value(int n, bool value) {
	if(value) {
		PORT->Group[0].OUTSET.reg = 1 << n;
	} else {
		PORT->Group[0].OUTCLR.reg = 1 << n;
	}
}

bool port_get_value(int n) {
	return (PORT->Group[0].IN.reg & (1<<n)) != 0;
}

// todo: sampling?

void port_set_pmux_enable(int n, bool enable) {	
	uint32_t value = PORT->Group[0].PINCFG[n].reg;
	if(enable) {
		value |= PORT_PINCFG_PMUXEN;
	} else {
		value &= ~PORT_PINCFG_PMUXEN;
	}
	PORT->Group[0].PINCFG[n].reg = value;
}

void port_set_function(int n, int function) {
	uint8_t value = PORT->Group[0].PMUX[n/2].reg;
	if(n&1) {
		value = (value & 0x0f) | ((function & 0xf) << 4);
	} else {
		value = (value & 0xf0) | (function & 0xf);
	}
	PORT->Group[0].PMUX[n/2].reg = value;
}

void port_set_pull_enable(int n, bool pulls) {
	if(pulls) {
		PORT->Group[0].PINCFG[n].reg |= PORT_PINCFG_PULLEN;
	} else {
		PORT->Group[0].PINCFG[n].reg &= ~PORT_PINCFG_PULLEN;
	}
}