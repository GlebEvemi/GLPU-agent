#include "api.h"
#include "service.h"

CURL *curl;
CURLcode result;

size_t gotData(char *buffer, size_t itemsize, size_t nitems, void *ignore);

int getData(){
    result = curl_global_init(CURL_GLOBAL_ALL);
    if(result != CURLE_OK)
        return (int)result;
    
    curl = curl_easy_init();

    if(curl) {
        FILE *fp = fopen("C:\\service_log_get_request.txt", "a");
        if(!fp) return -1;

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ivkh.ee/ru");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        Log("Before FUNC gotData!");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, gotData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        Log("After FUNC gotData!");
        result = curl_easy_perform(curl);
        if(result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(result));
        
        fclose(fp);
        curl_easy_cleanup(curl);
    }else{
        return -1;
    }

    curl_global_cleanup();
    return 0;
}

size_t gotData(char *buffer, size_t itemsize, size_t nitems, void *userdata){
    FILE *fp = (FILE*)userdata;
    size_t bytes = itemsize * nitems;

    fwrite(buffer, 1, bytes, fp);
    Log("Data saved");
    return bytes;
}