#include "request.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>


void request_list(void *list){

}

void get_key(char *key){
    FILE *file = fopen("lib/key.txt", "r");
    if (file == NULL){
        perror("Error opening file");
        exit(1);
    }

    char *f = fgets(key, sizeof(key), file);
    if(f == NULL){
        perror("Error reading file");
        exit(1);
    }
    key[strlen(key)] = '\0';

    fclose(file);

}
