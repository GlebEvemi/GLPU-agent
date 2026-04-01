#include "api.h"
#include "service.h"

CURL *curl;
CURLcode result;


int getData(){
    FILE *fp = fopen("C:\\service_log_get_request.txt", "a");
    result = curl_global_init(CURL_GLOBAL_ALL);
    if(result != CURLE_OK)
        return (int)result;
    
    curl = curl_easy_init();

    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ivkh.ee/ru");
        curt_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        if (fp)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, *fp);
        Log("Data is saved to C:\\service_log_get_request.txt");
    }


        result = curl_easy_perform(curl);
        if(result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(result));
        
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;

}