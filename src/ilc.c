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
void new(char *path, char *name);

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

void new(char *path, char *name){
  // monta fullpath sem '\0' para poder copiar limpo
  cArr_t fullpath = cArrNew(path, strlen(path));
  cArrPush(&fullpath, '/');
  for(u8 i = 0; i < strlen(name); i++) cArrPush(&fullpath, name[i]);

  // includePath
  cArr_t includePath = cArrCopy(fullpath);
  cArrPush(&includePath, '/');
  char include[] = "include";
  for(u16 i = 0; i < strlen(include); i++) cArrPush(&includePath, include[i]);
  cArrPush(&includePath, '\0');

  // srcPath
  cArr_t srcPath = cArrCopy(fullpath);
  cArrPush(&srcPath, '/');
  char src[] = "src";
  for(u16 i = 0; i < strlen(src); i++) cArrPush(&srcPath, src[i]);
  cArrPush(&srcPath, '\0');

  // buildPath
  cArr_t buildPath = cArrCopy(fullpath);
  cArrPush(&buildPath, '/');
  char build[] = "build";
  for(u16 i = 0; i < strlen(src); i++) cArrPush(&buildPath, build[i]);
  cArrPush(&buildPath, '\0');

  // filePath (ilc.toml)
  cArr_t filePath = cArrCopy(fullpath);
  cArrPush(&filePath, '/');
  char file[] = "ilc.toml";
  for(u16 i = 0; i < strlen(file); i++) cArrPush(&filePath, file[i]);
  cArrPush(&filePath, '\0');

  // termina o fullpath
  cArrPush(&fullpath, '\0');

  if(dirExists(fullpath.data)){
    printf("there is a folder with the same name of the project. Aborting...\n");
    cArrFree(&fullpath);
    cArrFree(&includePath);
    cArrFree(&srcPath);
    cArrFree(&filePath);
    cArrFree(&buildPath);
    return;
  }

  createDir(fullpath.data);
  createDir(includePath.data);
  createDir(srcPath.data);

  // cria ilc.toml
  fileInfo_t toml = fileOpen(filePath.data, "wb");
  char content[256];
  sprintf(content, "[project]\nname = \"%s\"\nversion = \"0.1.0\"\nflags = \"-O2\"\n[dependencies]", name);
  cFileWrite(&toml, content, strlen(content));
  fileClose(&toml);

  // monta main.c path a partir do srcPath já pronto
  cArr_t mainPath = cArrCopy(srcPath);
  mainPath.length--; // remove o '\0'
  cArrPush(&mainPath, '/');
  char mainFile[] = "main.c";
  for(u8 i = 0; i < strlen(mainFile); i++) cArrPush(&mainPath, mainFile[i]);
  cArrPush(&mainPath, '\0');

  // cria main.c
  fileInfo_t mainC = fileOpen(mainPath.data, "wb");
  char main[256];
  sprintf(main, "#include <stdio.h>\n\nint main(int argc, char *argv[]){\n    printf(\"Hello %s!\\n\");\n    return 0;\n}", name);
  cFileWrite(&mainC, main, strlen(main));
  fileClose(&mainC);

  cArrFree(&fullpath);
  cArrFree(&includePath);
  cArrFree(&srcPath);
  cArrFree(&filePath);
  cArrFree(&mainPath);
  cArrFree(&buildPath);
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
  if(strcmp(argv[1], "new") == 0){
    if(argc >= 3)
      new(curDir.path, argv[2]);
    else
      printf("there is no name for the new project\n");
  }
  
  return 0;
}

