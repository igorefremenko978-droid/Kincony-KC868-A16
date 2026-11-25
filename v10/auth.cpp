#include "auth.h"
#include "config.h"
#include <Preferences.h>

static Preferences prefs;
static const char* AUTH_NS = "auth_cfg";
static String s_user;
static String s_pass;
static String s_ap_ssid;
static String s_ap_pass;

void auth_init() {
    prefs.begin(AUTH_NS, true);
    s_user = prefs.getString("user", DEFAULT_USER);
    s_pass = prefs.getString("pass", DEFAULT_PASS);
    s_ap_ssid = prefs.getString("ap_ssid", DEFAULT_AP_SSID);
    s_ap_pass = prefs.getString("ap_pass", DEFAULT_AP_PASSWORD);
    prefs.end();
    
    Serial.printf("[AUTH] Loaded user: %s\n", s_user.c_str());
    Serial.printf("[AUTH] Loaded AP SSID: %s\n", s_ap_ssid.c_str());
}

bool auth_check(const String& user, const String& pass) {
    return (user == s_user && pass == s_pass);
}

void auth_set_credentials(const String& newUser, const String& newPass) {
    s_user = newUser;
    s_pass = newPass;
    prefs.begin(AUTH_NS, false);
    prefs.putString("user", s_user);
    prefs.putString("pass", s_pass);
    prefs.end();
    Serial.printf("[AUTH] Credentials updated: user=%s\n", s_user.c_str());
}

String auth_get_user() { return s_user; }
String auth_get_pass() { return s_pass; }

String auth_get_ap_ssid() { return s_ap_ssid; }
String auth_get_ap_pass() { return s_ap_pass; }

void auth_set_ap_config(const String& ssid, const String& pass) {
    s_ap_ssid = ssid;
    s_ap_pass = pass;
    prefs.begin(AUTH_NS, false);
    prefs.putString("ap_ssid", s_ap_ssid);
    prefs.putString("ap_pass", s_ap_pass);
    prefs.end();
    Serial.printf("[AUTH] AP config updated: SSID=%s\n", s_ap_ssid.c_str());
}
