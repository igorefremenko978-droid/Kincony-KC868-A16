#ifndef CONFIG_H
#define CONFIG_H

// --- Wi-Fi AP ---
#define DEFAULT_AP_SSID "PIE_Controller"
#define DEFAULT_AP_PASSWORD "12345678"
#define AP_IP IPAddress(192,168,4,1)
#define AP_GATEWAY IPAddress(192,168,4,1)
#define AP_SUBNET IPAddress(255,255,255,0)

// --- Ethernet (LAN8720 на KC868-A16) ---
// ВНИМАНИЕ: пин питания PHY на разных ревизиях: 16 или -1.
// Если у вас RS485 на 16/13 — питание PHY, скорее всего, не управляется (=-1).
#define ETH_PHY_ADDR    0
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
#define ETH_POWER_PIN   (-1)   // поставьте 16, если PHY требует питания
#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT

// --- I2C (KC868-A16: SDA=4, SCL=5) ---
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

// --- PCF8574 Адреса (7-бит) ---
#define PCF_INPUT_1_ADDR 0x22
#define PCF_INPUT_2_ADDR 0x23
#define PCF_OUTPUT_1_ADDR 0x24
#define PCF_OUTPUT_2_ADDR 0x25

// --- RS485 ---
#define RS485_RX_PIN 16
#define RS485_TX_PIN 13
#define RS485_DE_PIN (-1)

// --- Modbus ---
#define SENSOR_THS_ADDR 1
#define SENSOR_PT100_ADDR 2
#define THS_REG_HUMIDITY 0x0000
#define THS_REG_TEMP     0x0001
#define PT100_REG_TEMP   0x0000

// --- Отладка ---
#define MODBUS_DEBUG 0
#define POLL_INTERVAL_MS 2000

// --- Количество каналов ---
#define RELAY_COUNT 16
#define INPUT_COUNT 16

// --- Активные уровни ---
#define RELAY_ACTIVE_LOW 1
#define INPUT_ACTIVE_LOW 1

// --- Период опроса входов ---
#define INPUT_POLL_MS 100


// --- Идентификация устройства ---
#define HostName    "PIE_Controller"
#define FW_VERSION  "v1.0"

// --- Авторизация по умолчанию ---
#define DEFAULT_USER "admin"
#define DEFAULT_PASS "admin"

#endif
