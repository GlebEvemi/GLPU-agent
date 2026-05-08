#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <time.h>



//
int sendData(const char *server_url, const char *username, const char *password);

void getSystemInfoJson(char *buffer, size_t size);