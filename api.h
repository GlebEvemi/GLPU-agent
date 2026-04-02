#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <time.h>



//
int getData();

size_t gotData(char *buffer, size_t itemsize, size_t nitems, void *ignore);