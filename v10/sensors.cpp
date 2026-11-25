#include "sensors.h"
#include "config.h"
#include <ModbusMaster.h>

// --- Класс-обертка для перехвата байтов RS485 ---
class DebugSerial : public Stream {
private:
    HardwareSerial* _port;
public:
    DebugSerial(HardwareSerial* port) : _port(port) {}

    void begin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1, int8_t txPin = -1) {
        _port->begin(baud, config, rxPin, txPin);
    }

    int available() override { return _port->available(); }
    int peek() override { return _port->peek(); }
    
    int read() override {
        int val = _port->read();
#if MODBUS_DEBUG
        if (val >= 0) {
            Serial.printf("RX: %02X ", val);
        }
#endif
        return val;
    }

    size_t write(uint8_t val) override {
#if MODBUS_DEBUG
        Serial.printf("TX: %02X ", val);
#endif
        return _port->write(val);
    }

    size_t write(const uint8_t *buffer, size_t size) override {
#if MODBUS_DEBUG
        Serial.print("TX: ");
        for (size_t i = 0; i < size; i++) {
            Serial.printf("%02X ", buffer[i]);
        }
#endif
        return _port->write(buffer, size);
    }
};

// Создаем экземпляр нашего отладочного UART
DebugSerial debugSerial(&Serial2);

ModbusMaster node;
SensorData currentData;

void preTransmission() {
    if (RS485_DE_PIN >= 0) digitalWrite(RS485_DE_PIN, HIGH);
#if MODBUS_DEBUG
    Serial.print("\n[SENSORS] >> Send: ");
#endif
}

void postTransmission() {
    if (RS485_DE_PIN >= 0) digitalWrite(RS485_DE_PIN, LOW);
#if MODBUS_DEBUG
    Serial.println(); // Перенос строки после отправки
#endif
}

void sensors_init() {
    // Инициализируем физический порт
    Serial2.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

    if (RS485_DE_PIN >= 0) {
        pinMode(RS485_DE_PIN, OUTPUT);
        digitalWrite(RS485_DE_PIN, LOW);
    }

    // Передаем наш оберточный объект в ModbusMaster
    node.begin(SENSOR_THS_ADDR, debugSerial);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    currentData.last_update = 0;
    currentData.ths_valid = false;
    currentData.pt100_valid = false;

    Serial.println("[SENSORS] RS485 initialized.");
}

void sensors_poll(SensorData* data) {
    unsigned long now = millis();
    
    // Для отладки игнорируем интервал, чтобы шина работала непрерывно
#if !MODBUS_DEBUG
    if (now - data->last_update < POLL_INTERVAL_MS) return;
#endif
    data->last_update = now;

    Serial.println("\n[SENSORS] --- Polling cycle started ---");

    // --- THS датчик (адрес 1): влажность + температура ---
    node.begin(SENSOR_THS_ADDR, debugSerial);
    uint8_t result = node.readHoldingRegisters(THS_REG_HUMIDITY, 2);

    if (result == node.ku8MBSuccess) {
        uint16_t rawHum = node.getResponseBuffer(0);
        uint16_t rawTemp = node.getResponseBuffer(1);
        data->humidity_ths = rawHum / 10.0f;
        data->temperature_ths = rawTemp / 10.0f;
        data->ths_valid = true;
        Serial.printf("\n[SENSORS] THS OK: Hum=%.1f%%, Temp=%.1f C\n", data->humidity_ths, data->temperature_ths);
    } else {
        data->ths_valid = false;
        Serial.printf("\n[SENSORS] THS Error: 0x%02X\n", result);
    }

    delay(50); // Пауза между запросами к разным устройствам

    // --- PT100 датчик (адрес 2): температура ---
    node.begin(SENSOR_PT100_ADDR, debugSerial);
    
    if (RS485_DE_PIN >= 0) digitalWrite(RS485_DE_PIN, HIGH);
#if MODBUS_DEBUG
    Serial.print("[SENSORS] >> Send (PT100): ");
#endif

    result = node.readHoldingRegisters(PT100_REG_TEMP, 1);

#if MODBUS_DEBUG
    Serial.println();
#endif
    if (RS485_DE_PIN >= 0) digitalWrite(RS485_DE_PIN, LOW);

    if (result == node.ku8MBSuccess) {
        int16_t rawTemp = node.getResponseBuffer(0);
        data->temperature_pt100 = rawTemp / 10.0f;
        data->pt100_valid = true;
        Serial.printf("[SENSORS] PT100 OK: Temp=%.1f C\n", data->temperature_pt100);
    } else {
        data->pt100_valid = false;
        Serial.printf("[SENSORS] PT100 Error: 0x%02X\n", result);
    }

    // Синхронизация с глобальной копией
    currentData = *data;
}

SensorData sensors_get_data() {
    return currentData;
}