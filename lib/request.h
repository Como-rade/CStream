#ifndef REQUEST_H
#define REQUEST_H

#define KEY_MAX_LENGTH 1024
#define INPUT_LENGTH 1024
#define OPTION_MOVIE "movie"
#define OPTION_TV_SHOW "tv"

#include <stdbool.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <ctype.h>

typedef struct{
    char season[INPUT_LENGTH];
    char episode[INPUT_LENGTH];
}sae; //season and episode

void get_user_input();
/*
 Double const meaning: first one means the data pointed to this pointer may not be changed
 Second means the pointer itself may not be changed

 Here, since url is subject to change but the pointer itself not, it may be good practice to just put
 the second const.
 */
struct memory get_request_list(char *const url);
void show_list(char *url, char *const option, const char *name, sae sae);
void get_key(char *key);

#endif
