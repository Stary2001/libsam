#include "sam/gpio.h"
#include "sam/pinmux.h"

void pinmux_setup_usb() {
	port_set_direction(24, true);
	port_set_direction(25, true);
	port_set_value(24, false);
	port_set_value(25, false);
	port_set_pull_enable(24, false);
	port_set_pull_enable(25, false);

	port_set_pmux_enable(24, true);
	port_set_pmux_enable(25, true);
	port_set_function(24, 6); // G
	port_set_function(25, 6); // G
}