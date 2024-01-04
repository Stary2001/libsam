#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "sam.h"

template<int N> constexpr Tc * get_tc_ptr() {
#ifdef SAMD11
	static_assert(N != 0 && N < 3);

	if constexpr(N == 1) {
		return TC1;
	} else if(N == 2) {
		return TC2;
	}
#else
	static_assert(false);
	return nullptr;
#endif
}

template<int N> class TimerCounter {
public:
	void init();
	void set_duty_cycle(int chan, uint16_t n);
private:
	Tc *const tc_ptr = get_tc_ptr<N>();
};
