#include "storage.h"
#include <Arduino.h>

Preferences prefs;
const char* NVS_NAMESPACE = "pie_contorller";
const char* KEY_THEME = "theme_id";

void storage_init() {
    Serial.println("[NVS] Storage initialized.");
}

void storage_save_theme(int themeId) {
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putInt(KEY_THEME, themeId);
    prefs.end();
    Serial.printf("[NVS] Theme saved: %d\n", themeId);
}

int storage_load_theme() {
    prefs.begin(NVS_NAMESPACE, true);
    int theme = prefs.getInt(KEY_THEME, 0);
    prefs.end();
    Serial.printf("[NVS] Theme loaded: %d\n", theme);
    return theme;
}