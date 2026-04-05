#ifndef DIR_H
#define DIR_H

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <stdio.h>

#define SHOW_HIDDEN 1
#define DONT_SHOW_HIDDEN 0

typedef struct {
    DIR *dir;
    char *path;
    char *name;
} dirInfo_t;

u8 dirExists(const char *path);
dirInfo_t dirOpen(const char *path);
void dirDelete(const char *path);
char **dirList(const char *path, u8 show_hidden);
char *getCurrentDir();
#ifdef DIR_IMPLEMENTATION

u8 dirExists(const char *path){
  DIR *dir = opendir(path);
  if(dir != NULL){
    closedir(dir);
    return 1;
  }
    return 0;
}

dirInfo_t dirOpen(const char *path){
    dirInfo_t dir;  
    dir.dir = opendir((const char*)path);//verificar se dir j[a existe
    if(dir.dir ==NULL){
      return dir;
    }
    dir.path = path;
    
    char *last_slash = strrchr(path, '/');
    dir.name = last_slash ? strdup(last_slash + 1) : strdup(path);
    
    return dir;
}

u8 createDir(const char *path){
  return mkdir(path, 0755) == 0 ? 1 : 0;
}

void dirDelete(const char *path){
    rmdir(path);
}

char **dirList(const char *path, u8 show_hidden){

    struct dirent *entry;
    DIR *dp;
    char **archives = NULL;
    size_t size = 0;
    dp = opendir(path);
    if (dp == NULL){
      perror("opendir");
      return NULL;
    }
    
    if(show_hidden == SHOW_HIDDEN){
      while((entry = readdir(dp))){
        archives = realloc(archives, (++size + 1) * sizeof(char *)); //add 
        archives[size - 1] = strdup(entry->d_name);
      }
    }
    else if(show_hidden == DONT_SHOW_HIDDEN){
      while((entry = readdir(dp))){
        if(entry->d_name[0] != '.'){
          archives = realloc(archives, (++size + 1) * sizeof(char *));
          archives[size - 1] = strdup(entry->d_name);
        }
      }
    }
    archives[size] = NULL;
    closedir(dp);
    
    return archives;
}

char *getCurrentDir(){
  return getcwd(NULL, 0);
}


#endif
#endif /* DIR_H */
