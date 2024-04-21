#pragma once

void clock_switch_to_8mhz();
void clock_switch_to_48mhz_open_loop();
void clock_switch_to_48mhz_from_usb();

void clock_setup_gclk2_8mhz();
void hack_clock_setup_sercom0();
void hack_clock_setup_sercom1();
void hack_clock_setup_sercom2();
#ifndef SAMD11
void hack_clock_setup_sercom3();
#endif
void hack_clock_setup_adc();
void hack_clock_setup_tcs();
void hack_clock_setup_tcc0();
void clock_setup_usb();
void clock_setup_systick_1ms();
void clock_setup_xosc32k();
