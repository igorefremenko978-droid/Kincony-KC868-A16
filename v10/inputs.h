// inputs.h
#ifndef INPUTS_H
#define INPUTS_H
#include <Arduino.h>

void inputs_init();
void inputs_poll();
uint16_t inputs_get_mask();
bool inputs_get(int channel);

#endif