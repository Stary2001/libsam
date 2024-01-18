#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_A 0
#define PORT_B 1

void port_set_direction(unsigned int group, unsigned int n, bool value);
void port_set_value(unsigned int group, unsigned int n, bool value);
bool port_get_value(unsigned int group, unsigned int n);
void port_set_pmux_enable(unsigned int group, unsigned int n, bool enable);
void port_set_function(unsigned int group, unsigned int n, int function);
void port_set_pull_enable(unsigned int group, unsigned int n, bool enable);

#ifdef __cplusplus
}
#endif