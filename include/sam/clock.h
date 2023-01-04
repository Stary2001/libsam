#pragma once
#ifdef __cplusplus
extern "C" {
#endif

void clock_switch_to_8mhz();
void clock_switch_to_48mhz_open_loop();
void clock_switch_to_48mhz_from_usb();

void clock_setup_gclk2_8mhz();
void hack_clock_setup_sercom0();
void clock_setup_usb();

#ifdef __cplusplus
}
#endif