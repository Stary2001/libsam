#include <stdio.h>
#include "sam/clock.h"
#include "sam/tc.h"
#include "sam/gpio.h"
#include "sam.h"

/*
	The following bits need synchronization when written:
	* Software Reset bit in the Control A register (CTRLA.SWRST)
	* Enable bit in the Control A register (CTRLA.ENABLE)
	Write-synchronization is denoted by the Write-Synchronized property in the register description.
	The following registers need synchronization when written:
	* Control B Clear register (CTRLBCLR)
	* Control B Set register (CTRLBSET)
	* Control C register (CTRLC)
	* Count Value register (COUNT)
	* Period Value register (PERIOD)
	* Compare/Capture Value registers (CCx)
	Write-synchronization is denoted by the Write-Synchronized property in the register description.
	The following registers need synchronization when read:
	* Control B Clear register (CTRLBCLR)
	* Control B Set register (CTRLBSET)
	* Control C register (CTRLC)
	* Count Value register (COUNT)
	* Period Value register (PERIOD)
	* Compare/Capture Value registers (CCx)
	Read-synchronization is denoted by the Read-Synchronized property in the register description.
*/

// TODO: More than PWM...

template<int N> void TimerCounter<N>::init() {
	hack_clock_setup_tcs();

#ifdef SAMD11
	PM->APBCMASK.reg |= PM_APBCMASK_TC1;
	PM->APBCMASK.reg |= PM_APBCMASK_TC2;
#endif

	tc_ptr->COUNT16.CTRLA.bit.SWRST = 1;
	while (tc_ptr->COUNT16.STATUS.bit.SYNCBUSY) {};

	tc_ptr->COUNT16.CTRLA.reg = TC_CTRLA_WAVEGEN_NPWM | TC_CTRLA_MODE_COUNT16;
	tc_ptr->COUNT16.EVCTRL.reg = TC_EVCTRL_EVACT(0);
	tc_ptr->COUNT16.CC[1].reg = 0;

	// DIR
	// Set compare

	// Finally, enable in CTRLA
	tc_ptr->COUNT16.CTRLA.bit.ENABLE = 1;
	while (tc_ptr->COUNT16.STATUS.bit.SYNCBUSY) {};
}

template<int N> void TimerCounter<N>::set_duty_cycle(int chan, uint16_t n) {
	tc_ptr->COUNT16.CC[chan].reg = n;
	while (tc_ptr->COUNT16.STATUS.bit.SYNCBUSY) {};
}

#ifdef SAMD11
template class TimerCounter<1>;
template class TimerCounter<2>;
#endif
