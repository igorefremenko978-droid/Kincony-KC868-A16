#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include "sensors.h"

void web_server_init();
void web_server_update_data(const SensorData& data);

#endif
