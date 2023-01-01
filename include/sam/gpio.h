#include <stdint.h>
#include <stdbool.h>

void port_set_direction(int n, bool value);
void port_set_value(int n, bool value);
bool port_get_value(int n);
void port_set_pmux_enable(int n, bool enable);
void port_set_function(int n, int function);