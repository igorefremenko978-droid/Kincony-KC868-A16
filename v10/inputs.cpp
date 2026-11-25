// inputs.cpp
#include "inputs.h"
#include "config.h"
#include <Wire.h>

static uint16_t input_mask = 0;
static unsigned long last_poll = 0;

static uint8_t pcf_read(uint8_t addr) {
    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available()) return Wire.read();
    return 0xFF; // Если ошибка — считаем все входы неактивными (HIGH)
}

void inputs_init() {
    // I2C уже инициализирован в relays_init()
    // PCF8574 для входов не требует настройки pinMode (они всегда в режиме чтения)
    Serial.println("[INPUTS] PCF8574 inputs ready");
}

void inputs_poll() {
    unsigned long now = millis();
    if (now - last_poll < INPUT_POLL_MS) return;
    last_poll = now;

    uint8_t in1 = pcf_read(PCF_INPUT_1_ADDR); // IN 1-8
    uint8_t in2 = pcf_read(PCF_INPUT_2_ADDR); // IN 9-16

    // Инверсия для активного LOW (замкнут на GND = активен)
    uint16_t new_mask = 0;
    for (int i = 0; i < 8; i++) {
#if INPUT_ACTIVE_LOW
        if ((in1 & (1 << i)) == 0) new_mask |= (1 << i);
#else
        if (in1 & (1 << i)) new_mask |= (1 << i);
#endif
    }
    for (int i = 0; i < 8; i++) {
#if INPUT_ACTIVE_LOW
        if ((in2 & (1 << i)) == 0) new_mask |= (1 << (i + 8));
#else
        if (in2 & (1 << i)) new_mask |= (1 << (i + 8));
#endif
    }

    if (new_mask != input_mask) {
        Serial.printf("[INPUTS] Changed: 0x%04X -> 0x%04X\n",
                      (unsigned)input_mask, (unsigned)new_mask);
        input_mask = new_mask;
    }
}

uint16_t inputs_get_mask() { return input_mask; }

bool inputs_get(int channel) {
    if (channel < 0 || channel >= INPUT_COUNT) return false;
    return (input_mask >> channel) & 0x01;
}
