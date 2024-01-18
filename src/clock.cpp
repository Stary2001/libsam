#include "sam/clock.h"
#include "sam.h"

// clock code from:
// https://blog.thea.codes/understanding-the-sam-d21-clocks/

static uint32_t current_clock_speed = 1000000;

// Switch OSC8M to full speed
void clock_switch_to_8mhz() {
	SYSCTRL->OSC8M.bit.PRESC = 0x0;

	current_clock_speed = 8000000;
}

void clock_switch_to_48mhz_from_usb() {
	/* Set the correct number of wait states for 48 MHz @ 3.3v */
	NVMCTRL->CTRLB.bit.RWS = 1;

	/* This works around a quirk in the hardware (errata 1.2.1) -
	   the DFLLCTRL register must be manually reset to this value before
	   configuration. */
	while(!SYSCTRL->PCLKSR.bit.DFLLRDY);
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while(!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Write the coarse and fine calibration from NVM. */
	uint32_t coarse =
		((*(uint32_t*)FUSES_DFLL48M_COARSE_CAL_ADDR) & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;
	uint32_t fine =
		((*(uint32_t*)FUSES_DFLL48M_FINE_CAL_ADDR) & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;

	SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine);

	/* Wait for the write to finish. */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {};

	SYSCTRL->DFLLCTRL.reg |=
		/* Enable USB clock recovery mode */
		SYSCTRL_DFLLCTRL_USBCRM |
		/* Disable chill cycle as per datasheet to speed up locking.
		   This is specified in section 17.6.7.2.2, and chill cycles
		   are described in section 17.6.7.2.1. */
		SYSCTRL_DFLLCTRL_CCDIS;

	/* Configure the DFLL to multiply the 1 kHz clock to 48 MHz */
	SYSCTRL->DFLLMUL.reg =
		/* This value is output frequency / reference clock frequency,
		   so 48 MHz / 1 kHz */
		SYSCTRL_DFLLMUL_MUL(48000) |
		/* The coarse and fine values can be set to their minimum
		   since coarse is fixed in USB clock recovery mode and
		   fine should lock on quickly. */
		SYSCTRL_DFLLMUL_FSTEP(1) |
		SYSCTRL_DFLLMUL_CSTEP(1);

	/* Closed loop mode */
	SYSCTRL->DFLLCTRL.bit.MODE = 1;

	/* Enable the DFLL */
	SYSCTRL->DFLLCTRL.bit.ENABLE = 1;

	/* Wait for the write to complete */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {};

	/* Setup GCLK0 using the DFLL @ 48 MHz */
	GCLK->GENCTRL.reg =
		GCLK_GENCTRL_ID(0) |
		GCLK_GENCTRL_SRC_DFLL48M |
		/* Improve the duty cycle. */
		GCLK_GENCTRL_IDC |
		GCLK_GENCTRL_GENEN;

	/* Wait for the write to complete */
	while(GCLK->STATUS.bit.SYNCBUSY) {};

	current_clock_speed = 48000000;
}

void clock_setup_gclk2_8mhz() {
	/* Configure GCLK2's divider - in this case, no division - so just divide by one */
	GCLK->GENDIV.reg =
	    GCLK_GENDIV_ID(2) |
	    GCLK_GENDIV_DIV(1);

	/* Setup GCLK2 using the internal 8 MHz oscillator */
	GCLK->GENCTRL.reg =
	    GCLK_GENCTRL_ID(2) |
	    GCLK_GENCTRL_SRC_OSC8M |
	    /* Improve the duty cycle. */
	    GCLK_GENCTRL_IDC |
	    GCLK_GENCTRL_GENEN;

	/* Wait for the write to complete */
	while(GCLK->STATUS.bit.SYNCBUSY) {};
}

/* todo: closed loop with external crystal */

void hack_clock_setup_sercom0() {
	/* Connect GCLK2 to SERCOM0 */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK2 |
	    GCLK_CLKCTRL_ID_SERCOM0_CORE;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}
void hack_clock_setup_sercom1() {
	/* Connect GCLK2 to SERCOM1 */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK2 |
	    GCLK_CLKCTRL_ID_SERCOM1_CORE;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}
void hack_clock_setup_sercom2() {
	/* Connect GCLK2 to SERCOM1 */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK2 |
	    GCLK_CLKCTRL_ID_SERCOM2_CORE;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}
void hack_clock_setup_adc() {
	/* Connect GCLK2 to adc */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK2 |
	    GCLK_CLKCTRL_ID_ADC;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}

#ifdef SAMD11
void hack_clock_setup_tcs() {
	/* Connect GCLK2 to TCs */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK2 |
	    GCLK_CLKCTRL_ID_TC1_TC2;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}
void hack_clock_setup_tcc0() {
	/* Connect GCLK0 to TCs! */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK0 |
	    GCLK_CLKCTRL_ID_TCC0;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}
#endif

void clock_setup_usb() {
	PM->AHBMASK.reg |= PM_AHBMASK_USB;
	PM->APBBMASK.reg |= PM_APBBMASK_USB;

	/* Connect GCLK0 to USB */
	GCLK->CLKCTRL.reg =
	    GCLK_CLKCTRL_CLKEN |
	    GCLK_CLKCTRL_GEN_GCLK0 |
	    GCLK_CLKCTRL_ID_USB;

	/* Wait for the write to complete. */
	while (GCLK->STATUS.bit.SYNCBUSY) {};
}

void clock_setup_systick_1ms() {
	SysTick_Config(current_clock_speed / 1000);
}
