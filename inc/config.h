#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char server_url[256];
    char username[128];
    char password[128];
} Config;

int loadConfig(Config *config);

#endif