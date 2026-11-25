#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct SensorData {
    float temperature_ths;
    float humidity_ths;
    float temperature_pt100;
    bool ths_valid;
    bool pt100_valid;
    unsigned long last_update;
};

void sensors_init();
void sensors_poll(SensorData* data);
SensorData sensors_get_data();

#endif