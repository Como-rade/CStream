#include "request.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

struct memory {
  char *response;
  size_t size;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = nmemb;
  struct memory *mem = (struct memory *)clientp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
  return realsize;
}

void get_request_list(char *list_of_object){
    char key[REQUEST_LENGTH] = {0};
    get_key(key);
    struct memory chunk = {0};
    //printf("%s\n", key);

    struct curl_slist *header = NULL;
    header = curl_slist_append(header, key);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* cinit = curl_easy_init();

    if(cinit){
        CURLcode result;

        curl_easy_setopt(cinit, CURLOPT_URL, 
                "https://api.themoviedb.org/3/search/movie?query=Synecdoche%2C%20New%20York");
        curl_easy_setopt(cinit, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(cinit, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(cinit, CURLOPT_WRITEDATA, (void *)&chunk);

        result = curl_easy_perform(cinit);  
    }

    curl_slist_free_all(header);
    curl_easy_cleanup(cinit);
    curl_global_cleanup();

    cJSON *json = cJSON_Parse(chunk.response);
    char *string = cJSON_Print(json);
    printf("%s\n", string);
    cJSON *choonk = NULL;
    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    
    cJSON_ArrayForEach(choonk, results){
        printf("LOLOLOLOL");    
        cJSON *id = cJSON_GetObjectItemCaseSensitive(choonk, "id");
        printf("ID: %d\n", id->valueint);
    }

    printf("icitte\n");
    //printf("ID: %d\n", total_pages->valueint);
    free(json);
    //free(string);

    
    //printf("%s\n", key);
    //printf("pointer is: %c\n", *((char*)list+sizeof(char)));
    //printf("%ld\n", sizeof((char*)list));

}

void get_key(char *key){
    FILE *file = fopen("lib/key.txt", "r");
    if (file == NULL){
        perror("Error opening file");
        exit(1);
    }

    char *f = fgets(key, REQUEST_LENGTH, file);
    if(f == NULL){
        perror("Error reading file");
        exit(1);
    }
    printf("length: %ld\n", strlen(key));
    key[strlen(key)-1] = '\0';
    printf("length: %ld\n", strlen(key));

    fclose(file);

}

/*static size_t cb(char *data, size_t size, size_t nmemb, void *clientp){
    size_t realsize = nmemb;
    struct memory *mem = (struct memory *)clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(!ptr)
        exit(1);

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    return realsize;
}*/


