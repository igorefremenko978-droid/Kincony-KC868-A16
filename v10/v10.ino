/* Tools → Partition Scheme → "Huge APP (3MB No OTA/1MB SPIFFS)" */

#include <WiFi.h>
#include <ETH.h>
#include "config.h"
#include "storage.h"
#include "sensors.h"
#include "web_server.h"
#include "relays.h"
#include "inputs.h"
#include "net_config.h"
#include "ethernet_link.h"
#include "auth.h"          // <-- добавлено

SensorData sensorData;

static void startWifiAp() {
    WiFi.mode(WIFI_AP);
    WiFi.softAPsetHostname(HostName);
    WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
    
    // Берем настройки из памяти
    String ssid = auth_get_ap_ssid();
    String pass = auth_get_ap_pass();
    
    WiFi.softAP(ssid.c_str(), pass.c_str());
    Serial.printf("[WIFI] AP SSID: %s\n", ssid.c_str());
    Serial.print("[WIFI] AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.printf("\n\n=== %s %s (KC868-A16 Ethernet) ===\n", HostName, FW_VERSION);

    if (MODBUS_DEBUG) {
        Serial.println("[SYSTEM] MODBUS DEBUG MODE ENABLED!");
    }

    storage_init();
    auth_init();          // <-- инициализация авторизации
    net_config_init();
    relays_init();
    inputs_init();
    sensors_init();

    if (eth_link_init()) {
        NetConfig cfg = net_config_get();
        eth_link_apply(cfg);
    } else {
        Serial.println("[ETH] Init failed — работаем только через WiFi AP");
    }

    startWifiAp();

    web_server_init();

    sensors_poll(&sensorData);
    web_server_update_data(sensorData);

    Serial.println("=== System ready ===\n");
}

void loop() {
    inputs_poll();
    sensors_poll(&sensorData);
    web_server_update_data(sensorData);
    delay(5);
}
