#pragma once

template<int N> constexpr Sercom * get_sercom_ptr() {
	#ifdef SAMD11
	static_assert(N < 3);
	#else
	static_assert(N < 4);
	#endif

	if constexpr(N == 0) {
		return SERCOM0;
	} else if(N == 1) {
		return SERCOM1;
	} else if(N == 2) {
		return SERCOM2;
	}
	#ifndef SAMD11	
        else if(N == 3) {
        return SERCOM3;
    	}
	#endif
}
