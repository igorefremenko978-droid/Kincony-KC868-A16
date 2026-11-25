#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>
#include <IPAddress.h>

void storage_init();
void storage_save_theme(int themeId);
int  storage_load_theme();

// (опционально) сюда можно вынести net_config_* — но оставим отдельно

#endif