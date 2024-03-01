#pragma once

template<int N> constexpr Sercom * get_sercom_ptr() {
	static_assert(N < 4);

	if constexpr(N == 0) {
		return SERCOM0;
	} else if(N == 1) {
		return SERCOM1;
	} else if(N == 2) {
		return SERCOM2;
	} else if(N == 3) {
        return SERCOM3;
    }
}