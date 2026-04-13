#define ILCDIR_IMPLEMENTATION
#define ILCARRAY_IMPLEMENTATION
#define ILCFILE_IMPLEMENTATION
#define ILCSTRING_IMPLEMENTATION

#include <stdio.h>
#include "../include/ilcTypes.h"
//#include "../include/ilcArray.h"
#include "../include/ilcFile.h"
#include "../include/ilcDir.h"
#include <string.h>
#include "../include/ilcString.h"

void add(fileInfo_t project_info, char *lib_name);
void build(char *path);
void run(char *path);
void new(char *path, char *name);

void build(char *path){

  str_t files_path = strNew(path);
  strAppend(&files_path, "/src");
 
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
  strFree(&files_path);
  free(args);
}

void run(char *path){
  str_t bin_path = strNew(path);
  strAppend(&bin_path, "/build/run");
  i32 sucess = execl(bin_path.data, bin_path.data, NULL);
  strFree(&bin_path);
}

void new(char *path, char *name){
  
  str_t fullpath = strNew(path);
  strAppend(&fullpath, "/");
  strAppend(&fullpath, name);
  // includePath
  str_t includePath = strCopy(fullpath);
  strAppend(&includePath, "/include");
  
  // srcPath
  str_t srcPath = strCopy(fullpath);
  strAppend(&srcPath, "/src");

  // buildPath
  str_t buildPath = strCopy(fullpath);
  strAppend(&buildPath, "/build");

  // filePath (ilc.toml)
  str_t filePath = strCopy(fullpath);
  strAppend(&filePath, "/ilc.toml");

  if(dirExists(fullpath.data)){
    printf("there is a folder with the same name of the project. Aborting...\n");
    strFree(&fullpath);
    strFree(&includePath);
    strFree(&srcPath);
    strFree(&filePath);
    strFree(&buildPath);
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
  str_t mainPath = strCopy(srcPath);
  strAppend(&mainPath,"/main.c");

  // cria main.c
  fileInfo_t mainC = fileOpen(mainPath.data, "wb");
  char main[256];
  sprintf(main, "#include <stdio.h>\n\nint main(int argc, char *argv[]){\n    printf(\"Hello %s!\\n\");\n    return 0;\n}", name);
  cFileWrite(&mainC, main, strlen(main));
  fileClose(&mainC);

  strFree(&fullpath);
  strFree(&includePath);
  strFree(&srcPath);
  strFree(&filePath);
  strFree(&mainPath);
  strFree(&buildPath);
}

int main(int argc, char *argv[]){
  
  if(argc == 1){
    printf("Usage:\n - ilc run //run the project\n - ilc build //build the project\n - ilc add <library name> //add a library to the project\n");
    return 0;
  }  
  dirInfo_t curDir = dirOpen(getCurrentDir());
  //char **archives = dirList(curDir.path ,SHOW_HIDDEN);
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

