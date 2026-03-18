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

void get_user_input();
struct memory get_request_list(const char * const url);
bool show_list(const char * const url, const char * const option, 
        const char * const name);
void get_key(char *key);

#endif
