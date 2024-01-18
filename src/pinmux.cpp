#include "sam/gpio.h"
#include "sam/pinmux.h"

void pinmux_setup_usb() {
	port_set_direction(PORT_A, 24, true);
	port_set_direction(PORT_A, 25, true);
	port_set_value(PORT_A, 24, false);
	port_set_value(PORT_A, 25, false);
	port_set_pull_enable(PORT_A, 24, false);
	port_set_pull_enable(PORT_A, 25, false);

	port_set_pmux_enable(PORT_A, 24, true);
	port_set_pmux_enable(PORT_A, 25, true);
	port_set_function(PORT_A, 24, 6); // G
	port_set_function(PORT_A, 25, 6); // G
}