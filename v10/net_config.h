#ifndef NET_CONFIG_H
#define NET_CONFIG_H

#include <Arduino.h>
#include <IPAddress.h>

struct NetConfig {
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns;
    bool      useDhcp;   // на будущее; сейчас всегда false
    bool      valid;
};

void         net_config_init();
NetConfig    net_config_get();
bool         net_config_set(const IPAddress& ip,
                            const IPAddress& gw,
                            const IPAddress& mask,
                            const IPAddress& dns);
void         net_config_reset_default();

#endif