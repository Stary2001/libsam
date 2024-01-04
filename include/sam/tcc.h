#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"

template<int N> constexpr Tcc * get_tcc_ptr() {
	static_assert(N == 0);
	return TCC0;
}

template<int N> class TimerCounterControl {
public:
	void init();
	void set_duty_cycle(int chan, uint16_t n);
	void set_frequency(uint16_t n);
private:
	Tcc *const tcc_ptr = get_tcc_ptr<N>();
};
