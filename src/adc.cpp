#include <stdio.h>
#include "sam/clock.h"
#include "sam/adc.h"
#include "sam/gpio.h"
#include "sam.h"

void ADCClass::init() {
	hack_clock_setup_adc();

	/* Enable the APB clock for the ADC. */
	PM->APBCMASK.reg |= PM_APBCMASK_ADC;

	uint32_t bias = (*((uint32_t *) ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
	uint32_t linearity = (*((uint32_t *) ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
	linearity |= ((*((uint32_t *) ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;

	/* Wait for bus synchronization. */
	while (ADC->STATUS.bit.SYNCBUSY) {};

	/* Write the calibration data. */
	ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);
	
	/* Wait for bus synchronization. */
	while (ADC->STATUS.bit.SYNCBUSY) {};

	/* Use the internal VCC reference. This is 1/2 of what's on VCCA.
	   since VCCA is typically 3.3v, this is 1.65v.
	*/
	ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;

	/* Only capture one sample. The ADC can actually capture and average multiple
	   samples for better accuracy, but there's no need to do that for this
	   example.
	*/
	ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;

	/* Set the clock prescaler to 64, which will run the ADC at
	   8 Mhz / 64 = 250 kHz.
	   Set the resolution to 12bit.
	*/
	ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV64 |
	                 ADC_CTRLB_RESSEL_12BIT;

	// Set the sample time length to accommodate for input impedance.
	ADC->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(8);


	/* Wait for bus synchronization. */
	while (ADC->STATUS.bit.SYNCBUSY) {};

	/* Enable the ADC. */
	ADC->CTRLA.bit.ENABLE = true;
}

void ADCClass::select(unsigned int n) {
	/* Configure the input parameters.

	   - GAIN_DIV2 means that the input voltage is halved. This is important
	     because the voltage reference is 1/2 of VCCA. So if you want to
	     measure 0-3.3v, you need to halve the input as well.

	   - MUXNEG_GND means that the ADC should compare the input value to GND.

	   - MUXPOST_PIN3 means that the ADC should read from AIN3, or PB09.
	     This is A2 on the Feather M0 board.
	*/

	ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 |
	                     ADC_INPUTCTRL_MUXNEG_GND |
	                     ADC_INPUTCTRL_MUXPOS(n);
}

uint16_t ADCClass::read() {
	/* Wait for bus synchronization. */
	while (ADC->STATUS.bit.SYNCBUSY) {};

	/* Start the ADC using a software trigger. */
	ADC->SWTRIG.bit.START = true;

	/* Wait for the result ready flag to be set. */
	while (ADC->INTFLAG.bit.RESRDY == 0);

	/* Clear the flag. */
	ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

	/* Read the value. */
	return ADC->RESULT.reg;
}