#ifndef REQUEST_H
#define REQUEST_H

#define KEY_MAX_LENGTH 1024
#define INPUT_LENGTH 1024
#define OPTION_MOVIE "movie"
#define OPTION_TV_SHOW "tv"

void get_user_input();
struct memory get_request_list(const char * const url);
void show_list(const char * const url, const char * const option);
void get_key(char *key);

#endif
