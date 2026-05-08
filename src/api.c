#include "api.h"
#include "service.h"

CURL *curl;
CURLcode result;

void getSystemInfoJson(char *buffer, size_t size);

int sendData(const char *server_url,
             const char *username,
             const char *password)
{
    CURL *curl = NULL;

    char json[8192] = {0};

    getSystemInfoJson(json, sizeof(json));

    curl = curl_easy_init();

    if(!curl)
        return -1;

    char full_url[512];

    snprintf(full_url, sizeof(full_url), "%s/api/pc", server_url);

    curl_easy_setopt(curl, CURLOPT_URL, full_url);

    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(json));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode result = curl_easy_perform(curl);

    if(result != CURLE_OK) {
        fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(result));
    }

    curl_slist_free_all(headers);

    curl_easy_cleanup(curl);

    return (result == CURLE_OK) ? 0 : -1;
}

void getSystemInfoJson(char *buffer, size_t size) {
    FILE *fp = _popen("powershell -ExecutionPolicy Bypass -File getInfoScript.ps1", "r");
    if (!fp){
        perror("popen failed");
        return;
    }

    size_t total = 0;
    while (fgets(buffer + total, size - total, fp)) {
        total = strlen(buffer);
        if (total >= size - 1) break;
    }

    _pclose(fp);
    Log("Powershell has finished it's work");
}