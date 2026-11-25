// relays.cpp
#include "relays.h"
#include "config.h"
#include <Wire.h>

static uint16_t relay_mask = 0; // Логическое состояние (1 = ON)

// Запись байта в PCF8574 (для реле)
static void pcf_write(uint8_t addr, uint8_t data) {
    Wire.beginTransmission(addr);
    Wire.write(data);
    Wire.endTransmission();
}

void relays_init() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(100000); // 100 кГц стабильнее для PCF8574

    // Инициализация всех реле выключенными
    relay_mask = 0x0000;

    // PCF8574 при включении все пины в HIGH (слабая подтяжка).
    // Для KC868-A16 реле активны при LOW, значит "выключено" = HIGH.
    // Отправляем 0xFF (все HIGH) = все реле выключены.
    pcf_write(PCF_OUTPUT_1_ADDR, 0xFF);
    pcf_write(PCF_OUTPUT_2_ADDR, 0xFF);

    Serial.println("[RELAYS] PCF8574 outputs initialized (all OFF)");
}

void relays_set(int channel, bool on) {
    if (channel < 0 || channel >= RELAY_COUNT) return;

    if (on) relay_mask |= (1 << channel);
    else    relay_mask &= ~(1 << channel);

    // Разбиваем 16-битную маску на два байта
    uint8_t out1 = (relay_mask & 0xFF);         // Каналы 1-8 (PCF 0x24)
    uint8_t out2 = ((relay_mask >> 8) & 0xFF);  // Каналы 9-16 (PCF 0x25)

    // Инверсия для LOW-активных реле
#if RELAY_ACTIVE_LOW
    out1 = ~out1;
    out2 = ~out2;
#endif

    pcf_write(PCF_OUTPUT_1_ADDR, out1);
    pcf_write(PCF_OUTPUT_2_ADDR, out2);

    Serial.printf("[RELAYS] CH%d -> %s (mask=0x%04X)\n",
                  channel + 1, on ? "ON" : "OFF", (unsigned)relay_mask);
}

bool relays_get(int channel) {
    if (channel < 0 || channel >= RELAY_COUNT) return false;
    return (relay_mask >> channel) & 0x01;
}

uint16_t relays_get_mask() {
    return relay_mask;
}
