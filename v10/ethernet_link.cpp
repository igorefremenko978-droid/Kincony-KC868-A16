#include "ethernet_link.h"
#include "config.h"
#include <ETH.h>
#include <Network.h> // Core 3.x recommends using Network.h

static bool s_inited = false;
static bool s_connected = false;
static IPAddress s_ip(0,0,0,0);

// 1. Modify the event type to arduino_event_id_t
static void onEthEvent(arduino_event_id_t event) {
    switch (event) {
        case ARDUINO_EVENT_ETH_START:
            Serial.println("[ETH] Started");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("[ETH] Link UP");
            s_connected = true;
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            s_ip = ETH.localIP();
            Serial.printf("[ETH] Got IP: %s\n", s_ip.toString().c_str());
            s_connected = true;
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("[ETH] Link DOWN");
            s_connected = false;
            s_ip = IPAddress(0,0,0,0);
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("[ETH] Stopped");
            s_connected = false;
            break;
        default: break;
    }
}

bool eth_link_init() {
    if (s_inited) return true;

    // 2. Change to Network.onEvent
    Network.onEvent(onEthEvent);

    // 3. Adjust the parameter order of ETH.begin, passing ETH_PHY_LAN8720 as the first parameter
    // New signature: begin(type, phy_addr, mdc, mdio, power, clk_mode)
    bool ok = ETH.begin(ETH_PHY_LAN8720, // type
                        ETH_PHY_ADDR,    // phy_addr (your config is 0)
                        ETH_MDC_PIN,     // mdc
                        ETH_MDIO_PIN,    // mdio
                        ETH_POWER_PIN,   // power
                        ETH_CLK_MODE);   // clk_mode

    if (!ok) {
        Serial.println("[ETH] begin() failed");
        return false;
    }
    s_inited = true;
    Serial.println("[ETH] Initialized (waiting for link)");
    return true;
}

bool eth_link_apply(const NetConfig& cfg) {
    if (!s_inited) return false;

    if (!ETH.config(cfg.ip, cfg.gateway, cfg.subnet, cfg.dns)) {
        Serial.println("[ETH] config() failed");
        return false;
    }
    Serial.printf("[ETH] Applied static IP=%s\n", cfg.ip.toString().c_str());
    return true;
}

bool eth_link_is_connected() { return s_connected; }
IPAddress eth_link_ip()      { return s_ip; }