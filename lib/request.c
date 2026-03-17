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

struct memory get_request_list(char *list_of_object){
    char key[KEY_MAX_LENGTH] = {0};
    get_key(key);
    struct memory data = {0};

    struct curl_slist *header = NULL;
    header = curl_slist_append(header, key);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* cinit = curl_easy_init();

    if(cinit){
        CURLcode result;

        curl_easy_setopt(cinit, CURLOPT_URL, 
                "https://api.themoviedb.org/3/search/movie?query=Eternal%20Sunshine%20of%20the%20Spotless%20Mind");
        curl_easy_setopt(cinit, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(cinit, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(cinit, CURLOPT_WRITEDATA, (void *)&data);

        result = curl_easy_perform(cinit);  
    }

    curl_slist_free_all(header);
    curl_easy_cleanup(cinit);
    curl_global_cleanup();
    printf("%s\n", data.response);

    return data;
}

void show_list(){

    struct memory data = get_request_list(NULL);
    int counter = 1;
    cJSON *json = cJSON_Parse(data.response);
    char *string = cJSON_Print(json);
    printf("%s\n", string);
    cJSON *chunk = NULL;
    cJSON *all_list = (cJSON *)calloc(1, sizeof(*all_list));
    cJSON *temp = NULL;
    cJSON *head = all_list;
    
    cJSON *total_results = cJSON_GetObjectItemCaseSensitive(json, "total_results");
    printf("is number %d\n", total_results->valueint);
    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    
    
    cJSON_ArrayForEach(chunk, results){
        //printf("LOLOLOLOL");
        /*temp = cJSON_GetArrayItem(choonk, 3);
        all_list->valueint = temp->valueint;
        temp = cJSON_GetArrayItem(choonk, 5);
        all_list->valuestring = temp->valuestring;*/
        temp = cJSON_GetObjectItemCaseSensitive(chunk, "id");   
        all_list->valueint = temp->valueint;
        temp = cJSON_GetObjectItemCaseSensitive(chunk, "original_title");
        all_list->valuestring = temp->valuestring;
        temp = cJSON_GetObjectItemCaseSensitive(chunk, "overview");
        all_list->string = temp->valuestring;
        
        if(counter <= total_results->valueint){
            printf("here\n");
            all_list->next = (cJSON *)calloc(1, sizeof(*all_list));
            all_list = all_list->next;
            counter++;
        }else{
            break;
        }
        /*if(all_list->next == NULL){
            printf("Aint nothing");
        }*/
        /*printf("ID: %d\n"
               "Valuestring: %s\n"
               "String: %s\n"
                , all_list->valueint, all_list->valuestring, all_list->string);*/
    }
    all_list = head;
    while(all_list->next != NULL){
        printf("ID: %d\n"
               "Valuestring: %s\n"
               "String: %s\n"
                , all_list->valueint, all_list->valuestring, all_list->string);
        all_list = all_list->next;
    }
    printf("array size: %d\n", cJSON_GetArraySize(results));

    //printf("ID: %d\n", total_pages->valueint);
    free(json);
    free(string);
    free(chunk);
    free(temp);
    free(total_results);
    all_list = head;
   // while(head->next != NULL){
        //head = all_list->next;
        free(all_list);
    //}
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

    char *f = fgets(key, KEY_MAX_LENGTH, file);
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


