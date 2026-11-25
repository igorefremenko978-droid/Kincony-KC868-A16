// relays.h
#ifndef RELAYS_H
#define RELAYS_H
#include <Arduino.h>

void relays_init();
void relays_set(int channel, bool on); // 0..15
bool relays_get(int channel);
uint16_t relays_get_mask();

#endif