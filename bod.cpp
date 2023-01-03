#include "sam/bod.h"
#include "sam.h"

// https://blog.thea.codes/sam-d21-brown-out-detector/
// <3

void bod_init() {
	/* Disable the brown-out detector during configuration,
	   otherwise it might misbehave and reset the
	   microcontroller. */
	SYSCTRL->BOD33.bit.ENABLE = 0;
	while (!SYSCTRL->PCLKSR.bit.B33SRDY) {};

	/* Configure the brown-out detector so that the
	   program can use it to watch the power supply
	   voltage */
	SYSCTRL->BOD33.reg = (
	    /* This sets the minimum voltage level to 3.0v - 3.2v.
	       See datasheet table 37-21. */
	    SYSCTRL_BOD33_LEVEL(48) |
	    /* Since the program is waiting for the voltage to rise,
	       don't reset the microcontroller if the voltage is too
	       low. */
	    SYSCTRL_BOD33_ACTION_NONE |
	    /* Enable hysteresis to better deal with noisy power
	       supplies and voltage transients. */
	    SYSCTRL_BOD33_HYST);

	/* Enable the brown-out detector and then wait for
	   the voltage level to settle. */
	SYSCTRL->BOD33.bit.ENABLE = 1;
	while (!SYSCTRL->PCLKSR.bit.BOD33RDY) {}

	/* BOD33DET is set when the voltage is *too low*,
	   so wait for it to be cleared. */
	while (SYSCTRL->PCLKSR.bit.BOD33DET) {}
}

void bod_set_3v3() {
	SYSCTRL->BOD33.bit.ENABLE = 0;
	while (!SYSCTRL->PCLKSR.bit.B33SRDY) {};

	SYSCTRL->BOD33.reg |= SYSCTRL_BOD33_ACTION_RESET;

	SYSCTRL->BOD33.bit.ENABLE = 1;
}