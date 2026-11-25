#include "net_config.h"
#include <Preferences.h>

static Preferences prefs;
static const char* NS = "net_cfg";

static NetConfig cfg;

static bool ip_is_valid(const IPAddress& ip) {
    // Не 0.0.0.0, не 255.255.255.255, не multicast
    uint32_t v = (uint32_t)ip;
    if (v == 0 || v == 0xFFFFFFFF) return false;
    if ((v & 0xF0000000UL) == 0xE0000000UL) return false; // multicast
    return true;
}

static void load_defaults() {
    cfg.ip      = IPAddress(192, 168, 1, 100);
    cfg.gateway = IPAddress(192, 168, 1, 1);
    cfg.subnet  = IPAddress(255, 255, 255, 0);
    cfg.dns     = IPAddress(192, 168, 1, 1);
    cfg.useDhcp = false;
    cfg.valid   = false;
}

void net_config_init() {
    load_defaults();
    prefs.begin(NS, true);
    if (prefs.isKey("ip")) {
        cfg.ip      = IPAddress(prefs.getUInt("ip",     0));
        cfg.gateway = IPAddress(prefs.getUInt("gw",     0));
        cfg.subnet  = IPAddress(prefs.getUInt("mask",   0));
        cfg.dns     = IPAddress(prefs.getUInt("dns",    0));
        cfg.valid   = ip_is_valid(cfg.ip) && ip_is_valid(cfg.subnet);
    }
    prefs.end();
    Serial.printf("[NETCFG] IP=%s GW=%s MASK=%s DNS=%s valid=%d\n",
                  cfg.ip.toString().c_str(),
                  cfg.gateway.toString().c_str(),
                  cfg.subnet.toString().c_str(),
                  cfg.dns.toString().c_str(),
                  (int)cfg.valid);
}

NetConfig net_config_get() { return cfg; }

bool net_config_set(const IPAddress& ip,
                    const IPAddress& gw,
                    const IPAddress& mask,
                    const IPAddress& dns) {
    if (!ip_is_valid(ip) || !ip_is_valid(mask)) {
        Serial.println("[NETCFG] Reject: bad IP/mask");
        return false;
    }
    if (gw != IPAddress(0,0,0,0) && !ip_is_valid(gw)) {
        Serial.println("[NETCFG] Reject: bad gateway");
        return false;
    }
    cfg.ip = ip; cfg.gateway = gw; cfg.subnet = mask; cfg.dns = dns;
    cfg.valid = true;
    cfg.useDhcp = false;

    prefs.begin(NS, false);
    prefs.putUInt("ip",   (uint32_t)ip);
    prefs.putUInt("gw",   (uint32_t)gw);
    prefs.putUInt("mask", (uint32_t)mask);
    prefs.putUInt("dns",  (uint32_t)dns);
    prefs.end();

    Serial.printf("[NETCFG] Saved IP=%s GW=%s MASK=%s DNS=%s\n",
                  ip.toString().c_str(), gw.toString().c_str(),
                  mask.toString().c_str(), dns.toString().c_str());
    return true;
}

void net_config_reset_default() {
    prefs.begin(NS, false);
    prefs.clear();
    prefs.end();
    load_defaults();
    Serial.println("[NETCFG] Reset to defaults");
}
