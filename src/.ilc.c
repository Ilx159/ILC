#define DIR_IMPLEMENTATION
#define ARRAY_IMPLEMENTATION
#define FILE_IMPLEMENTATION

#include <stdio.h>
#include "../include/types.h"
#include "../include/array.h"
#include "../include/file.h"
#include "../include/dir.h"

void add(fileInfo_t project_info, char *lib_name);
void build(char *path);
void run(char *path);

void build(char *path){

  cArr_t files_path = cArrNew(path, strlen(path));
    char path2[] = "/src";
    for(int i = 0; i < strlen(path2); i++){
      cArrPush(&files_path, path2[i]);
    }
  cArrPush(&files_path, '\0');

  
  char **files = dirList(files_path.data, DONT_SHOW_HIDDEN); 
  size_t count = 0;
  while(files[count] != NULL) count++;
  
  char **args = malloc((count + 6)*sizeof(char*));

  for(size_t i = 0; i < count; i++){
    args[i+1] = malloc(files_path.length+strlen(files[i]) + 2);
    sprintf(args[i+1], "%s/%s",files_path.data , files[i]); 
  }
  args[0] = "gcc";
  args[count + 1] = "-Wall";
  args[count + 2] = "-Wextra";
  args[count + 3] = "-o";
  args[count + 4] = "build/run";
  args[count + 5] = NULL;


  int sucess = execv("/usr/bin/gcc",args);
  cArrFree(&files_path);
  free(args);
}

void run(char *path){

  cArr_t bin_path = cArrNew(path, strlen(path));
  char path2[] = "/build/run";
  for(int i = 0; i < strlen(path2); i++){
    cArrPush(&bin_path, path2[i]);
  }
  cArrPush(&bin_path, '\0');
  i32 sucess = execl(bin_path.data, bin_path.data, NULL);
  cArrFree(&bin_path);
}




int main(int argc, char *argv[]){
  
  if(argc == 1){
    printf("Usage:\n - ilc run //run the project\n - ilc build //build the project\n - ilc add <library name> //add a library to the project\n");
    return 0;
  }  
  dirInfo_t curDir = dirOpen(getCurrentDir());
  char **archives = dirList(curDir.path ,SHOW_HIDDEN);
  if(strcmp(argv[1], "run") == 0) run(curDir.path);
  if(strcmp(argv[1], "build") == 0) build(curDir.path);
  
  
  return 0;
}
