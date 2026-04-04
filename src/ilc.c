#include <stdio.h>
#include "../include/types.h"
#include "../include/array.h"
#include "../include/file.h"
#include "../include/dir.h"

#define DIR_IMPLEMENTATION
#define ARRAY_IMPLEMENTATION
#define FILE_IMPLEMENTATION

void add(fileInfo_t project_info, char *lib_name);
char *build(const char*Includes, const char *files, const char*);
void run(const char *path);

void run(const char *path){
  
  char *bin_path = malloc();

}


int main(int argc, char *argv[]){
  #define arg(num) argv[argc - num]
  
  if(argc == 1){
    printf("Usage:\n - ilc run //run the project\n - ilc build //build the project\n - ilc add <library name> //add a library to the project\n");
    return 0;
  }  
  dirInfo_t curDir = dirOpen(getCurrentDir());
  char **archives = dirList(curDir.path ,SHOW_HIDDEN);
  if(strcmp(arg(2), "run")) run(curDir.path);
  printf("%s", arg(1));



  
  return 0;
}
