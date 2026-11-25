#ifndef AUTH_H
#define AUTH_H

#include <Arduino.h>

void auth_init();
bool auth_check(const String& user, const String& pass);
void auth_set_credentials(const String& newUser, const String& newPass);
String auth_get_user();
String auth_get_pass();

// Новое для Wi-Fi AP
String auth_get_ap_ssid();
String auth_get_ap_pass();
void auth_set_ap_config(const String& ssid, const String& pass);

#endif
