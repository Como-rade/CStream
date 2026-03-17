#ifndef REQUEST_H
#define REQUEST_H
#define KEY_MAX_LENGTH 1024

struct memory get_request_list(char *list_of_object);
void show_list();
void get_key(char *key);

#endif
