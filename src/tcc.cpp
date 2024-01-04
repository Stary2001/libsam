#include <stdio.h>
#include "sam/clock.h"
#include "sam/tcc.h"
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

template<int N> void TimerCounterControl<N>::init() {
	hack_clock_setup_tcc0();

	PM->APBCMASK.reg |= PM_APBCMASK_TCC0;

	tcc_ptr->CTRLA.bit.SWRST = 1;
	while (tcc_ptr->SYNCBUSY.bit.SWRST) {};

	tcc_ptr->CTRLA.reg = TCC_CTRLA_PRESCALER_DIV1 | TCC_CTRLA_PRESCSYNC_PRESC;
	tcc_ptr->WAVE.reg = TCC_WAVE_WAVEGEN(TCC_WAVE_WAVEGEN_NPWM_Val);
	tcc_ptr->PER.reg = 2047;

	// Finally, enable in CTRLA
	tcc_ptr->CTRLA.bit.ENABLE = 1;
	while (tcc_ptr->SYNCBUSY.bit.ENABLE) {};
}

template<int N> void TimerCounterControl<N>::set_duty_cycle(int chan, uint16_t n) {
	tcc_ptr->COUNT.reg = 0;
	tcc_ptr->CC[chan].reg = n;
	// TODO: fuck, which bit
	while (tcc_ptr->SYNCBUSY.bit.CC1) {};
}

template<int N> void TimerCounterControl<N>::set_frequency(uint16_t n) {
	tcc_ptr->PER.reg = n;
	while (tcc_ptr->SYNCBUSY.bit.PER) {};
}

template class TimerCounterControl<0>;