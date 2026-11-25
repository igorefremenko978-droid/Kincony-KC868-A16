// web_server.cpp
#include "web_server.h"
#include "web_ui.h"
#include "storage.h"
#include "relays.h"
#include "inputs.h"
#include "net_config.h"
#include "ethernet_link.h"
#include "auth.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ETH.h>
#include "config.h"
#include <mbedtls/base64.h> // для декодирования Base64

AsyncWebServer server(80);
SensorData lastSensorData;
int currentThemeId = 0;

// Проверка авторизации (Basic Auth)
bool requireAuth(AsyncWebServerRequest *req) {
    String clientIP = req->client()->remoteIP().toString();
    
    // Если заголовка нет — просим авторизоваться (без лога, чтобы не спамить)
    if (!req->hasHeader("Authorization")) {
        req->requestAuthentication();
        return false;
    }

    String authHeader = req->header("Authorization");
    String username = "unknown";
    
    // Извлекаем логин для лога
    if (authHeader.startsWith("Basic ")) {
        String encoded = authHeader.substring(6);
        size_t decodedLen = 0;
        mbedtls_base64_decode(NULL, 0, &decodedLen, (const unsigned char*)encoded.c_str(), encoded.length());
        if (decodedLen > 0) {
            unsigned char* decoded = (unsigned char*)malloc(decodedLen + 1);
            if (decoded) {
                if (mbedtls_base64_decode(decoded, decodedLen, &decodedLen, 
                                          (const unsigned char*)encoded.c_str(), encoded.length()) == 0) {
                    decoded[decodedLen] = '\0';
                    String creds = String((char*)decoded);
                    int colon = creds.indexOf(':');
                    if (colon != -1) username = creds.substring(0, colon);
                }
                free(decoded);
            }
        }
    } else if (authHeader.startsWith("Digest ")) {
        int start = authHeader.indexOf("username=\"");
        if (start != -1) {
            start += 10;
            int end = authHeader.indexOf('"', start);
            if (end != -1) username = authHeader.substring(start, end);
        }
    } else {
        Serial.printf("[AUTH] Unknown auth type from %s\n", clientIP.c_str());
        req->requestAuthentication();
        return false;
    }

    // Проверяем пароль встроенным методом
    if (!req->authenticate(auth_get_user().c_str(), auth_get_pass().c_str())) {
        Serial.printf("[AUTH] FAILED: user='%s' from %s (expected user='%s')\n",
                      username.c_str(), clientIP.c_str(), auth_get_user().c_str());
        req->requestAuthentication();
        return false;
    }

    // Успешная авторизация — НЕ ЛОГИРУЕМ, чтобы не спамить.
    // Если нужен лог, раскомментируйте строку ниже (но будет много сообщений):
    // Serial.printf("[AUTH] OK: user='%s' from %s\n", username.c_str(), clientIP.c_str());
    
    return true;
}

void web_server_init() {
    currentThemeId = storage_load_theme();

    // --- Главная страница ---
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        AsyncWebServerResponse *response = req->beginResponse_P(200, "text/html", HTML_PAGE);
        response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        response->addHeader("Pragma", "no-cache");
        response->addHeader("Expires", "0");
        req->send(response);
    });

    // --- Телеметрия ---
    server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        JsonDocument doc;
        doc["temperature_ths"]   = lastSensorData.temperature_ths;
        doc["humidity_ths"]      = lastSensorData.humidity_ths;
        doc["temperature_pt100"] = lastSensorData.temperature_pt100;
        doc["ths_valid"]         = lastSensorData.ths_valid;
        doc["pt100_valid"]       = lastSensorData.pt100_valid;
        doc["relay_mask"]        = relays_get_mask();
        doc["input_mask"]        = inputs_get_mask();
        doc["uptime"]            = millis() / 1000;

        doc["wifi_ip"]           = WiFi.softAPIP().toString();
        doc["eth_connected"]     = eth_link_is_connected();
        doc["eth_ip"]            = eth_link_ip().toString();

        String out;
        serializeJson(doc, out);
        req->send(200, "application/json", out);
    });

    // --- Реле ---
    server.on("/api/relay", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        if (!req->hasParam("ch") || !req->hasParam("state")) {
            req->send(400, "text/plain", "Bad params"); return;
        }
        int ch = req->getParam("ch")->value().toInt();
        int st = req->getParam("state")->value().toInt();
        if (ch < 0 || ch >= RELAY_COUNT) {
            req->send(400, "text/plain", "Bad channel"); return;
        }
        relays_set(ch, st != 0);
        req->send(200, "text/plain", "OK");
    });

    // --- Тема ---
    server.on("/api/theme", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        if (req->hasParam("set")) {
            int nt = req->getParam("set")->value().toInt();
            if (nt >= 0 && nt <= 3) {
                currentThemeId = nt;
                storage_save_theme(nt);
            }
            req->send(200, "text/plain", "OK");
        } else {
            JsonDocument doc;
            doc["theme"] = currentThemeId;
            String out;
            serializeJson(doc, out);
            req->send(200, "application/json", out);
        }
    });

    // --- Сетевые настройки: чтение ---
    server.on("/api/net", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        NetConfig cfg = net_config_get();
        JsonDocument doc;
        doc["ip"]      = cfg.ip.toString();
        doc["gateway"] = cfg.gateway.toString();
        doc["subnet"]  = cfg.subnet.toString();
        doc["dns"]     = cfg.dns.toString();
        doc["valid"]   = cfg.valid;
        String out;
        serializeJson(doc, out);
        req->send(200, "application/json", out);
    });

    // --- Сетевые настройки: запись ---
    auto saveNetHandler = [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        Serial.println("\n[WEB] === /api/net/save request received ===");
        
        if (!req->hasParam("ip", true) || !req->hasParam("mask", true)) {
            Serial.println("[WEB] Error: Missing ip/mask parameters!");
            req->send(400, "text/plain", "Missing ip/mask"); return;
        }
        
        String ipStr   = req->getParam("ip", true)->value();
        String maskStr = req->getParam("mask", true)->value();
        String gwStr   = req->hasParam("gw", true)  ? req->getParam("gw", true)->value()  : "";
        String dnsStr  = req->hasParam("dns", true) ? req->getParam("dns", true)->value() : "";
        
        ipStr.trim(); maskStr.trim(); gwStr.trim(); dnsStr.trim();
        
        Serial.printf("[WEB] Parsed: IP='%s', Mask='%s', GW='%s', DNS='%s'\n", 
                      ipStr.c_str(), maskStr.c_str(), gwStr.c_str(), dnsStr.c_str());

        IPAddress ip, mask, gw(0,0,0,0), dns(0,0,0,0);
        
        if (!ip.fromString(ipStr)) {
            Serial.printf("[WEB] Error: Bad IP string '%s'\n", ipStr.c_str());
            req->send(400, "text/plain", "Bad IP format"); return;
        }
        if (!mask.fromString(maskStr)) {
            Serial.printf("[WEB] Error: Bad MASK string '%s'\n", maskStr.c_str());
            req->send(400, "text/plain", "Bad MASK format"); return;
        }
        if (gwStr.length() > 0 && !gw.fromString(gwStr)) {
            Serial.printf("[WEB] Error: Bad GW string '%s'\n", gwStr.c_str());
            req->send(400, "text/plain", "Bad GW format"); return;
        }
        if (dnsStr.length() > 0 && !dns.fromString(dnsStr)) {
            Serial.printf("[WEB] Error: Bad DNS string '%s'\n", dnsStr.c_str());
            req->send(400, "text/plain", "Bad DNS format"); return;
        }

        if (!net_config_set(ip, gw, mask, dns)) {
            Serial.println("[WEB] Error: net_config_set failed (validation)");
            req->send(400, "text/plain", "Validation failed"); return;
        }

        NetConfig cfg = net_config_get();
        bool applied = eth_link_apply(cfg);

        JsonDocument doc;
        doc["ok"]      = true;
        doc["applied"] = applied;
        String out;
        serializeJson(doc, out);
        
        Serial.printf("[WEB] Success! IP=%s Applied=%d\n", ip.toString().c_str(), applied);
        req->send(200, "application/json", out);
    };

    server.on("/api/net/save", HTTP_GET, saveNetHandler);
    server.on("/api/net/save", HTTP_POST, saveNetHandler);

    // --- Сброс сети ---
    server.on("/api/net/reset", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        net_config_reset_default();
        NetConfig cfg = net_config_get();
        bool applied = eth_link_apply(cfg);
        JsonDocument doc;
        doc["ok"] = true;
        doc["applied"] = applied;
        String out;
        serializeJson(doc, out);
        req->send(200, "application/json", out);
    });

    // --- Авторизация: чтение логина ---
    server.on("/api/auth", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        JsonDocument doc;
        doc["user"] = auth_get_user();
        String out;
        serializeJson(doc, out);
        req->send(200, "application/json", out);
    });

    // --- Авторизация: смена логина/пароля ---
    server.on("/api/auth/save", HTTP_POST, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        if (!req->hasParam("user", true) || !req->hasParam("pass", true)) {
            req->send(400, "text/plain", "Missing user/pass"); return;
        }
        String nu = req->getParam("user", true)->value();
        String np = req->getParam("pass", true)->value();
		if (nu.length() < 3 || np.length() < 3) {
			req->send(400, "text/plain", "Too short"); return;
		}
		auth_set_credentials(nu, np);
		Serial.printf("[AUTH] Password changed: user='%s' from %s\n", 
					  nu.c_str(), req->client()->remoteIP().toString().c_str()); // <-- добавлено
		req->send(200, "application/json", "{\"ok\":true}");
    });

	    // --- Wi-Fi AP: чтение ---
    server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        JsonDocument doc;
        doc["ssid"] = auth_get_ap_ssid();
        doc["pass"] = auth_get_ap_pass(); // Отправляем пароль, так как это локальная сеть и авторизация уже пройдена
        String out;
        serializeJson(doc, out);
        req->send(200, "application/json", out);
    });

    // --- Wi-Fi AP: запись ---
    server.on("/api/wifi/save", HTTP_POST, [](AsyncWebServerRequest *req){
        if (!requireAuth(req)) return;
        if (!req->hasParam("ssid", true) || !req->hasParam("pass", true)) {
            req->send(400, "text/plain", "Missing params"); return;
        }
        String ssid = req->getParam("ssid", true)->value();
        String pass = req->getParam("pass", true)->value();
        
        if (ssid.length() < 1 || ssid.length() > 32 || pass.length() < 8 || pass.length() > 63) {
            req->send(400, "text/plain", "Bad SSID or Pass length (pass >= 8)"); return;
        }

        auth_set_ap_config(ssid, pass);
        Serial.printf("[AUTH] Wi-Fi AP changed by %s. Rebooting...\n", req->client()->remoteIP().toString().c_str());
        
        req->send(200, "application/json", "{\"ok\":true}");
        
        // Даем время отправить ответ, затем перезагружаем
        delay(1000);
        ESP.restart();
    });
	
    server.begin();
    Serial.println("[WEB] Server started on port 80 (auth enabled)");
}

void web_server_update_data(const SensorData& data) {
    lastSensorData = data;
}
