#include <stdio.h>
#include "lib/request.h"
#include <string.h>

int main(void){
    char isfiddy[50] = {'a','b'};
    //printf("main pointer: %c\n size of : %ld\n", isfiddy[1], sizeof(isfiddy));
    //get_request_list((char*)isfiddy);
    //void *lol;
    //printf("size of lol : %ld\n", sizeof(lol));
    show_list();

    return 0;
}
