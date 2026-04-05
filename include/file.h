#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <unistd.h>

#define XDATA_TYPES\
  X(u8, uint8_t, 1)\
  X(u16, uint16_t, 2)\
  X(u32, uint32_t, 4)\
  X(u64, uint64_t, 8)\
  X(i8, int8_t, 1)\
  X(i16, int16_t, 2)\
  X(i32, int32_t, 4)\
  X(i64, int64_t, 8)\
  X(f32, float, 4)\
  X(f64, double, 8)\
  X(c, char, 1)

typedef struct {
  FILE *file;
  size_t size;
  char *path;
  char *name;
  char *ext;  
} fileInfo_t;

fileInfo_t fileOpen(char *path, char *mode);
void fileClose(fileInfo_t *fileI);
#define X(name, type, num_bytes)\
  type * name##FileRead(fileInfo_t *fileI);\
  void name##FileWrite(fileInfo_t *fileI, const type *data, size_t lenght);
XDATA_TYPES
#undef X
u8 fileExists(const char* path);
void fileDelete(const char* path);
void fileRename(char* path, char* name);
void fileMove(const char* origin, const char* dest);
fileInfo_t fileCopy(fileInfo_t fileI, char* dest);

#ifdef FILE_IMPLEMENTATION

fileInfo_t fileOpen(char *path, char *mode){
  fileInfo_t fileI = {.path = path};

  fileI.file = fopen(path, mode);
  if(fileI.file == NULL){
    exit(EXIT_FAILURE);
  }
  fseek(fileI.file, 0, SEEK_END);
  fileI.size = ftell(fileI.file);
  rewind(fileI.file);

  char *last_slash = strrchr(path, '/');
  char *filename = last_slash ? last_slash + 1 : path;
  char *dot = strrchr(filename, '.');
  
  if(dot){
    size_t name_len = dot - filename;
    fileI.name = (char*)malloc(name_len + 1);
    strncpy(fileI.name, filename, name_len);
    fileI.name[name_len] = '\0';
    fileI.ext = strdup(dot + 1);
  } else {
    fileI.name = strdup(filename);
    fileI.ext = NULL;
  }
  return fileI;
}

void fileClose(fileInfo_t *fileI){
  if(fileI->file){
    fclose(fileI->file);
    fileI->file = NULL;
  }
  free(fileI->name);
  free(fileI->ext);
}

#define X(name, type, num_bytes)\
type * name##FileRead(fileInfo_t *fileI){\
  type *buffer = (type *)malloc(fileI->size);\
  if(buffer == NULL) exit(EXIT_FAILURE);\
  fread(buffer, num_bytes, fileI->size/num_bytes, fileI->file);\
  return buffer;\
}\
void name##FileWrite(fileInfo_t *fileI, const type* data, size_t length){\
    fwrite(data, num_bytes, length, fileI->file);\
}
XDATA_TYPES
#undef X

void fileDelete(const char* path){
    unlink(path);
}

void fileRename(char* path, char* name){
  char *tmp = strrchr(path, '/');
  size_t index = tmp - path;
  char *newPath = (char *)malloc(strlen(path) + 1);
  memcpy(newPath, path, strlen(path) + 1);
  newPath = (char *)realloc(newPath, index + strlen(name) + 1);
  memcpy(&newPath[index+1], name, strlen(name));
  newPath[index + 1 + strlen(name)] = 0;
  rename(path, newPath);
  free(newPath);
}

void fileMove(const char* origin, const char* dest){
  rename(origin, dest);
}

//you can rename the file here when coping!!!
fileInfo_t fileCopy(fileInfo_t fileI, char* dest){
    char *new_path = malloc(strlen(dest) + 2);
    FILE *test_file = fopen(dest, "r");
    if(test_file != NULL){ //if a file with the smae name exist, now edits the name adding a zero in the end, \
                           //but in the future this will return an fileInfo_t with NULL vars and with an ERROR signal.
      fclose(test_file);
      memcpy(new_path, dest, strlen(dest));
      new_path[strlen(dest)] = '0';
      new_path[strlen(dest) + 1] = 0; 
    } else {
      free(new_path);
      new_path = dest;
    }
  
    fileInfo_t new_file = {.file = fopen(dest, "wb")};
    new_file.ext = strdup(fileI.ext);
    new_file.path = new_path;
    new_file.name = strdup(fileI.name);
    //resolver com memcpy dps
    u8 *tmp = u8FileRead(&fileI);
    u8FileWrite(&new_file, tmp, fileI.size);
    free(tmp);

    return new_file;
}

u8 fileExists(const char* path){
    return access(path, F_OK) == 0 ? 1 : 0;
}

#endif
#endif /* FILE_H */
