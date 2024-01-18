#include "sam/gpio.h"
#include "sam.h"

// 0 = input
// 1 = output
void port_set_direction(unsigned int group, unsigned int n, bool value) {
	if(value) {
		PORT->Group[group].DIRSET.reg = 1 << n;
	} else {
		PORT->Group[group].DIRCLR.reg = 1 << n;
	}
}

void port_set_value(unsigned int group, unsigned int n, bool value) {
	if(value) {
		PORT->Group[group].OUTSET.reg = 1 << n;
	} else {
		PORT->Group[group].OUTCLR.reg = 1 << n;
	}
}

bool port_get_value(unsigned int group, unsigned int n) {
	return (PORT->Group[group].IN.reg & (1<<n)) != 0;
}

// todo: sampling?

void port_set_pmux_enable(unsigned int group, unsigned int n, bool enable) {
	uint32_t value = PORT->Group[group].PINCFG[n].reg;
	if(enable) {
		value |= PORT_PINCFG_PMUXEN;
	} else {
		value &= ~PORT_PINCFG_PMUXEN;
	}
	PORT->Group[group].PINCFG[n].reg = value;
}

void port_set_function(unsigned int group, unsigned int n, int function) {
	uint8_t value = PORT->Group[group].PMUX[n/2].reg;
	if(n&1) {
		// PMUX O
		value = (value & 0x0f) | ((function & 0xf) << 4);
	} else {
		// PMUX E
		value = (value & 0xf0) | (function & 0xf);
	}
	PORT->Group[group].PMUX[n/2].reg = value;
}

void port_set_pull_enable(unsigned int group, unsigned int n, bool pulls) {
	if(pulls) {
		PORT->Group[group].PINCFG[n].reg |= PORT_PINCFG_PULLEN;
	} else {
		PORT->Group[group].PINCFG[n].reg &= ~PORT_PINCFG_PULLEN;
	}
}