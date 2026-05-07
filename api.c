#include "api.h"
#include "service.h"

CURL *curl;
CURLcode result;

void getSystemInfoJson(char *buffer, size_t size);

int sendData(){
    char json[8192] = {0};
    getSystemInfoJson(json, sizeof(json));

    if(result != CURLE_OK)
        return (int)result;
    
    curl = curl_easy_init();

    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ivkh.ee/ru"); //https://ipadress
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(json));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "GLPU-Agent/1.0");

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        result = curl_easy_perform(curl);

        if(result != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(result));
            Log("Problem on stage of sending data to server");
            Log("Service stopped");
            curl_easy_cleanup(curl);
            UpdateStatus(SERVICE_STOPPED);
            return -1;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        Log("The data has been sent");
    }else{
        return -1;
    }

    curl_global_cleanup();
    return 0;
}

void getSystemInfoJson(char *buffer, size_t size) {
    FILE *fp = _popen("powershell -ExecutionPolicy Bypass -File getInfoScript.ps1", "r");
    if (!fp) {
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