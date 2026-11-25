#ifndef ETHERNET_LINK_H
#define ETHERNET_LINK_H

#include <Arduino.h>
#include "net_config.h"

bool eth_link_init();
bool eth_link_apply(const NetConfig& cfg);   // применить IP без реинициализации
bool eth_link_is_connected();
IPAddress eth_link_ip();

#endif
