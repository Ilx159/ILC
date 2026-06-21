#ifndef ILCDIR_H
#define ILCDIR_H

#include "ilcTypes.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHOW_HIDDEN 1
#define DONT_SHOW_HIDDEN 0

typedef struct {
  DIR *dir;
  char *path;
  char *name;
} dirInfo_t;

typedef struct {
  char **itensPaths;
  size_t numItens;
} pathsList_t;

u8 dirExists(const char *path);
dirInfo_t dirOpen(const char *path);
void dirDelete(const char *path);
pathsList_t dirList(const char *path, u8 showHidden);
char *dirGetCurrent();
u8 dirCreate(const char *path);
void freePathsList(pathsList_t *list);
pathsList_t dirListRecursive(const char *path, u8 showHidden);

#ifdef ILCDIR_IMPLEMENTATION

static u8 dirIsFile(const char *path) {
  struct stat st;
  if (stat(path, &st) != 0)
    return 0;
  return S_ISREG(st.st_mode) ? 1 : 0;
}

static char *pathAppend(const char *dest, const char *src) {
  size_t len = strlen(dest) + 1 + strlen(src) + 1;
  char *result = (char *)malloc(len);
  snprintf(result, len, "%s/%s", dest, src);
  return result;
}

u8 dirExists(const char *path) {
  DIR *dir = opendir(path);
  if (dir != NULL) {
    closedir(dir);
    return 1;
  }
  return 0;
}

dirInfo_t dirOpen(const char *path) {
  dirInfo_t dir;
  dir.dir = opendir((const char *)path);
  if (dir.dir == NULL) {
    return dir;
  }

  dir.path = (char *)path;

  char *last_slash = strrchr((char *)path, '/');
  dir.name = last_slash ? strdup(last_slash + 1) : strdup(path);

  return dir;
}

u8 dirCreate(const char *path) { return mkdir(path, 0755) == 0 ? 1 : 0; }

void dirDelete(const char *path) { rmdir(path); }

pathsList_t dirList(const char *path, u8 showHidden) {

  struct dirent *entry;
  DIR *dp;
  pathsList_t archives;
  archives.itensPaths = NULL;
  archives.numItens = 0;
  dp = opendir(path);
  if (dp == NULL) {
    perror("opendir");
    return (pathsList_t){
        .numItens = 0,
        .itensPaths = NULL,
    };
  }

  while ((entry = readdir(dp))) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
    if (showHidden == DONT_SHOW_HIDDEN && entry->d_name[0] == '.')
      continue;
    archives.itensPaths = (char **)realloc(
        archives.itensPaths, (++archives.numItens) * sizeof(char *));
    archives.itensPaths[archives.numItens - 1] =
        pathAppend(path, entry->d_name);
  }
  closedir(dp);

  return archives;
}

void freePathsList(pathsList_t *list) {
  for (size_t i = 0; i < list->numItens; i++) {
    free(list->itensPaths[i]);
    list->itensPaths[i] = NULL;
  }
  free(list->itensPaths);
  list->itensPaths = NULL;
  list->numItens = 0;
}

char *dirGetCurrent() { return getcwd(NULL, 0); }

pathsList_t dirListRecursive(const char *path, u8 showHidden) {
  pathsList_t list = dirList(path, showHidden);
  size_t step = 0;
  while (step < list.numItens) {
    if (dirIsFile(list.itensPaths[step])) {
      step++;
      continue;
    }
    if (dirExists(list.itensPaths[step])) {
      pathsList_t temp = dirList(list.itensPaths[step], showHidden);
      size_t newTotal = list.numItens + temp.numItens;
      list.itensPaths =
          (char **)realloc(list.itensPaths, sizeof(char *) * newTotal);
      for (size_t i = list.numItens, j = 0; list.numItens < (i + temp.numItens);
           list.numItens++, j++) {
        list.itensPaths[list.numItens] = strdup(temp.itensPaths[j]);
      }
      freePathsList(&temp);
      step++;
      continue;
    }
  }
  return list;
}

#endif
#endif /* DIR_H */
