## CStream

This is a simple program written in C that let's you watch streaming movie through your browser.
Nothing fancy purely to practice C.
Use of AI will be none. 

### Dependancies
- lcurl development package [download](https://everything.curl.dev/install/linux.html)
- brave is preferred [download](https://brave.com/linux/)

### Libraries/Headers
- cJSON.h/.c [doc](https://github.com/DaveGamble/cJSON) / [other useful link](https://www.geeksforgeeks.org/c/cjson-json-file-write-read-modify-in-c/)
- curl/curl.h [doc](https://curl.se/libcurl/c/libcurl-tutorial.html)

### STANDARDS

For all the naming standards, I will be following [C Coding Standard by CMU](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html)

### Log on logs
- [x] Added cJSON library, finished curl request
- [x] Completed fetching data from TMDB
- [x] Completed user input
- [x] Watch movies    
- [x] Watch TV shows  
To do:  
- [ ] Implement some sort of cache
- [ ] Choice to view all the options fetched
- [ ] Implement some sort of CLI
- [ ] Factoring
- [ ] Create a Make or CMake

___

To run it:  
In the main directory;  
gcc main.c lib/\*.c -c  
gcc \*.o -o main  
./main  

Get a free API key on [TMDB](https://www.themoviedb.org/?language=en-CA)  
Then create a `key.txt` file in `lib/` directory and pass in the **API Read Access Token**   




