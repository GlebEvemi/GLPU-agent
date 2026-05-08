#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "cJSON.h"

int loadConfig(Config *config)
{
    FILE *fp = fopen("C:\\ProgramData\\GLPU-Agent\\config.json", "r");

    if(!fp) {
        printf("Failed to open config\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *jsonData = malloc(size + 1);

    if(!jsonData) {
        fclose(fp);
        return -1;
    }

    fread(jsonData, 1, size, fp);
    jsonData[size] = '\0';

    fclose(fp);

    cJSON *json = cJSON_Parse(jsonData);

    if(!json) {
        printf("JSON parse error\n");
        free(jsonData);
        return -1;
    }

    cJSON *server = cJSON_GetObjectItem(json, "server_url");
    cJSON *user   = cJSON_GetObjectItem(json, "username");
    cJSON *pass   = cJSON_GetObjectItem(json, "password");

    if(!cJSON_IsString(server) ||
       !cJSON_IsString(user) ||
       !cJSON_IsString(pass))
    {
        printf("Invalid config format\n");

        cJSON_Delete(json);
        free(jsonData);

        return -1;
    }

    strncpy(config->server_url, server->valuestring,
            sizeof(config->server_url));

    strncpy(config->username, user->valuestring,
            sizeof(config->username));

    strncpy(config->password, pass->valuestring,
            sizeof(config->password));

    cJSON_Delete(json);
    free(jsonData);

    return 0;
}