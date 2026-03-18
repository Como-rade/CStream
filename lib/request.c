#include "request.h"

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
    return 0;

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
  return realsize;
}

void get_url(const long int id, const char * const option){
    char url[1024] = "xdg-open https://www.vidking.net/embed/";
    char its[10] = {0};
    sprintf(its, "%ld", id);
    if(!strcasecmp(option, OPTION_MOVIE)){
        strcat(url, option);
        url[strlen(url)] = '/';
        strcat(url, its);
        strcat(url, "?autoPlay=true");
        url[strlen(url)] = '\0';
        printf("%s\n", url);
        system(url);
    }

}

void string_clean(char *string){
    fgets(string, INPUT_LENGTH, stdin);
    string[strlen(string)-1] ='\0';
}

void string_to_query(char *string, char *query){
    char hex[16] = {0};
    for(int i = 0; string[i] != '\0'; i++){
        switch(string[i]){
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

    do{
        memset(url, 0, sizeof(url));
        memset(query, 0, sizeof(query));
        memset(choice_input, 0, sizeof(choice_input));
        memset(name_input, 0, sizeof(name_input));

        strncpy(url, "https://api.themoviedb.org/3/search/", sizeof(url));
        strncpy(query, "?query=", sizeof(query));

        printf("Would you like to search for a TV show or movie?"
                "(type tv or movie)\n");
        string_clean(choice_input);
        //printf("%d\n%d\n",strcasecmp(choice_input, OPTION_MOVIE),strcasecmp(choice_input, OPTION_TV_SHOW));
        while(strcasecmp(choice_input, OPTION_MOVIE) && strcasecmp(choice_input, OPTION_TV_SHOW)){
            printf("Choose a valid option. Type tv or move.\n");
            memset(choice_input, 0, sizeof(choice_input));
            string_clean(choice_input);
        }
        //printf("%s\n", choice_input);

        printf("Enter the name of the %s\n", (strcasecmp(choice_input, OPTION_MOVIE)? "tv show" : "movie" ));
        string_clean(name_input);
        string_to_query(name_input, query);
        make_full_url(strcasecmp(choice_input, OPTION_MOVIE)? OPTION_TV_SHOW : OPTION_MOVIE, query, url);
        //printf("%s\n", url);
        //show_list(url, choice_input, name_input);
    }while(show_list(url, choice_input, name_input));

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

    return data;
}

bool show_list(const char * const url, const char * const option, 
        const char * const name){

    struct memory data = get_request_list(url);
    long int id = 0;
    int counter = 1;
    cJSON *json = cJSON_Parse(data.response);
    /*char *string = cJSON_Print(json);
    printf("%s\n", string);*/
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
    
    if(total_results->valueint > 0){
        cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");    
    
        cJSON_ArrayForEach(chunk, results){
            temp = cJSON_GetObjectItemCaseSensitive(chunk, "id");   
            all_list->valueint = temp->valueint;
            temp = cJSON_GetObjectItemCaseSensitive(chunk, 
                    (strcasecmp(option, OPTION_MOVIE)? "original_name": "original_title"));
            all_list->valuestring = temp->valuestring;
            if(!strcasecmp(name, all_list->valuestring)) id = (long int)all_list->valueint; break;
            temp = cJSON_GetObjectItemCaseSensitive(chunk, "overview");
            all_list->string = temp->valuestring;
        
            if(counter <= total_results->valueint){
                all_list->next = (cJSON *)calloc(1, sizeof(*all_list));
                all_list = all_list->next;
                counter++;
            }else{
                break;
            }
        }

        all_list = head;
        free(json);
        //free(string);
        free(chunk);
        free(temp);
        free(total_results);
        all_list = head;
        free(all_list);
    
        json = NULL;
        //string = NULL;
        chunk = NULL;
        temp = NULL;
        total_results = NULL;
        all_list = NULL;
        get_url(id, option);
        return false;
    }else{
        printf("No result, please try again.\n");
        return true;
    }
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
    //printf("length: %ld\n", strlen(key));
    key[strlen(key)-1] = '\0';
    //printf("length: %ld\n", strlen(key));

    fclose(file);

}

