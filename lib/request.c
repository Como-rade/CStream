#include "request.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <ctype.h>

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

void string_clean(char *string){
    fgets(string, INPUT_LENGTH, stdin);
    string[strlen(string)-1] ='\0';
    //printf("%s\n", string);
}

void string_to_query(char *string, char *query){
    char hex[16] = {0};
    /*sprintf(hex, "%x", '@');
    printf("lol is%s\n", hex);*/
    for(int i = 0; string[i] != '\0'; i++){
        switch(string[i]){
            /*case ' ': strcat(query, "%20");
                      break;
            case '*': strcat(query, "%2A");
                      break;
            case '+': strcat(query, "%2B");
                      break;
            case '#': strcat(query, "%23");
                      break;
            case '$': strcat(query, "%24");
                      break;
            case '!': strcat(query, "%21");
                      break;
            case '(': strcat(query, "%28");
                      break;
            case ')': strcat(query, "%29");
                      break;
            case '<': strcat(query, "%3C");
                      break;
            case '>': strcat(query, "%3E");
                      break;
            case '@': strcat(query, "%40");
                      break;*/
            case (char)-73: strcat(query, "%B7");
                            break;
            case (char)-62: strcat(query, "%C2");
                      break;
            case (char)-67: strcat(query,"%BD");
                      break;
            default: if(isalpha(string[i])){
                        query[strlen(query)] = string[i];
                     }else{
                        sprintf(hex, "%%%x", string[i]);
                        strcat(query, hex);
                        memset(hex, 0, strlen(hex));
                     }
        }
    }
    memset(hex, 0, strlen(hex));
    printf("%s\n", query);
}

void make_full_url(char *option, char *query, char *url){
    strcat(url, option);
    strcat(url, query); 
}

void get_user_input(){
    char url[INPUT_LENGTH] = "https://api.themoviedb.org/3/search/";
    char query[INPUT_LENGTH] = "?query=";
    char choice_input[INPUT_LENGTH] = {0};
    char name_input[INPUT_LENGTH] = {0};
    
    printf("Would you like to search for a TV show or movie?"
            "(type tv or movie)\n");
    string_clean(choice_input);
    printf("%d\n%d\n",strcasecmp(choice_input, OPTION_MOVIE),strcasecmp(choice_input, OPTION_TV_SHOW));
    while(strcasecmp(choice_input, OPTION_MOVIE) && strcasecmp(choice_input, OPTION_TV_SHOW)){
        printf("Choose a valid option. Type tv or move.\n");
        memset(choice_input, 0, sizeof(choice_input));
        string_clean(choice_input);
    }
    printf("%s\n", choice_input);

    printf("Enter the name of the %s\n", (strcasecmp(choice_input, OPTION_MOVIE)? "tv show" : "movie" ));
    string_clean(name_input);
    string_to_query(name_input, query);
    make_full_url(strcasecmp(choice_input, OPTION_MOVIE)? OPTION_TV_SHOW : OPTION_MOVIE, query, url);
    printf("%s\n", url);
    show_list(url, choice_input);

}

struct memory get_request_list(char const * const url){
    char key[KEY_MAX_LENGTH] = {0};
    get_key(key);
    struct memory data = {0};

    struct curl_slist *header = NULL;
    header = curl_slist_append(header, key);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* cinit = curl_easy_init();

    if(cinit){
        CURLcode result;

        curl_easy_setopt(cinit, CURLOPT_URL, url);
        curl_easy_setopt(cinit, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(cinit, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(cinit, CURLOPT_WRITEDATA, (void *)&data);

        result = curl_easy_perform(cinit);  
    }

    curl_slist_free_all(header);
    curl_easy_cleanup(cinit);
    curl_global_cleanup();
    //printf("%s\n", data.response);

    return data;
}

void show_list(const char * const url, const char * const option){

    struct memory data = get_request_list(url);
    //printf("%s\n", data.response);
    int counter = 1;
    cJSON *json = cJSON_Parse(data.response);
    char *string = cJSON_Print(json);
    printf("%s\n", string);
    if(json == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
      exit(1);  
    }
    cJSON *chunk = NULL;
    cJSON *all_list = (cJSON *)calloc(1, sizeof(*all_list));
    cJSON *temp = NULL;
    cJSON *head = all_list;
    
    cJSON *total_results = cJSON_GetObjectItemCaseSensitive(json, "total_results");
    printf("is number %d\n", total_results->valueint);
    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    printf("array size: %d\n", cJSON_GetArraySize(results));
    //printf("icicicic\n");
    //printf("what what%s\n", results->string);
    
    
    cJSON_ArrayForEach(chunk, results){
        printf("LOLOLOLOL");
        /*temp = cJSON_GetArrayItem(choonk, 3);
        all_list->valueint = temp->valueint;
        temp = cJSON_GetArrayItem(choonk, 5);
        all_list->valuestring = temp->valuestring;*/
        temp = cJSON_GetObjectItemCaseSensitive(chunk, "id");   
        all_list->valueint = temp->valueint;
        temp = cJSON_GetObjectItemCaseSensitive(chunk, 
                (strcasecmp(option, OPTION_MOVIE)? "original_name": "original_title"));
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


