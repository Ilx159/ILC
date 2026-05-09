#define ILCDIR_IMPLEMENTATION
#define ILCARRAY_IMPLEMENTATION
#define ILCFILE_IMPLEMENTATION
#define ILCSTRING_IMPLEMENTATION

#include "../include/ilcArray.h"
#include "../include/ilcDir.h"
#include "../include/ilcFile.h"
#include "../include/ilcString.h"
#include "../include/ilcTypes.h"
#include <stdio.h>
#include <string.h>

#define PATH_MAX 4096
#define FILE_NAME_MAX 512
#define LIB_MAX 512

void add(fileInfo_t project_info, char lib_name[LIB_MAX]);
void build(char path[PATH_MAX]);
void run(char path[PATH_MAX]);
void new(char path[PATH_MAX], char name[FILE_NAME_MAX]);

void build(char path[PATH_MAX]) {


  str_t files_path = strNew(path);

  // TOML

  str_t toml = strNew(path);

  strAppend(&toml, "/ilc.toml");

  fileInfo_t tomlFile = fileOpen(toml.data, "rb");

  char *tomlData = cFileRead(&tomlFile);
  str_t tomlDataStr = strNew(tomlData);

  size_t numStrings;
  str_t *parsedContent = strSplit(&tomlDataStr, '\n', &numStrings);
  size_t argsPos = 0;
  for (; argsPos <= numStrings; argsPos++) {
    if (strStartWith(&parsedContent[argsPos], "flags=")) {
      break;
    }
  }
  
  if (argsPos == numStrings) {
    printf("\nThere is no compile_flags in ilc.toml!\n");
    return;
  }

  str_t flagsLine = parsedContent[argsPos];
  strStrip(&flagsLine, ' ');
  
  size_t numFlags = 0;
  str_t *flags = strSplit(&flagsLine, '-', &numFlags);
    

  cArrPop((cArr_t *)&flags[numFlags-1]);
  numFlags--;
  for(int i = 0; i < numFlags; i++){
    printf("%s\n",flags[i+1].data);
    fflush(stdout);
  }

  // list src
  strAppend(&files_path, "/src");

  char **files = dirList(files_path.data, DONT_SHOW_HIDDEN);
  size_t count = 0;
  while (files[count] != NULL)
    count++;

  char **args = malloc((count + 4 + numFlags) * sizeof(char *));
  
  printf("%zu", count);

  for (size_t i = 0; i < count; i++) {
    args[i + 1] = malloc(files_path.length + strlen(files[i]) + 2);
    sprintf(args[i + 1], "%s/%s", files_path.data, files[i]);
  }
  
  for(size_t i = 0; i < count; i++){
    printf("%s\n", args[i + 1]);
    fflush(stdout);
  }
  args[0] = "gcc";
  for (size_t i = 0; i < numFlags; i++) {
    cArrInsert((cArr_t *)&flags[i+1], 0, '-');
    args[count + i + 1] = flags[i+1].data;
  }
  

  args[count + numFlags + 1] = "-o";
  args[count + numFlags + 2] = "build/run";
  args[count + numFlags + 3] = NULL;
  
    for (int i = 0; args[i] != NULL; i++) {
        printf("%s ", args[i]);

    fflush(stdout);
    }

  if(execvp(args[0], args)){
    perror("error on building");
  }

  for(size_t i = 0; i < numStrings; i++){
    strFree(&parsedContent[i]);
  }

  strFree(&files_path);
  free(args);
}

// AAAAAAAAAA



void run(char path[PATH_MAX]) {
  str_t bin_path = strNew(path);
  strAppend(&bin_path, "/build/run");
  i32 sucess = execl(bin_path.data, bin_path.data, NULL);
  strFree(&bin_path);
}

void new(char path[PATH_MAX], char name[FILE_NAME_MAX]) {

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

  if (dirExists(fullpath.data)) {
    printf(
        "there is a folder with the same name of the project. Aborting...\n");
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
  createDir(buildPath.data);

  // cria ilc.toml
  fileInfo_t toml = fileOpen(filePath.data, "wb");
  char content[256];
  sprintf(content,
          "[project]\nname = \"%s\"\nversion = \"0.1.0\"\nflags = "
          "\"-O2\"\n[dependencies]",
          name);
  cFileWrite(&toml, content, strlen(content));
  fileClose(&toml);

  // monta main.c path a partir do srcPath já pronto
  str_t mainPath = strCopy(srcPath);
  strAppend(&mainPath, "/main.c");

  // cria main.c
  fileInfo_t mainC = fileOpen(mainPath.data, "wb");
  char main[256];
  sprintf(main,
          "#include <stdio.h>\n\nint main(int argc, char *argv[]){\n    "
          "printf(\"Hello %s!\\n\");\n    return 0;\n}",
          name);
  cFileWrite(&mainC, main, strlen(main));
  fileClose(&mainC);

  strFree(&fullpath);
  strFree(&includePath);
  strFree(&srcPath);
  strFree(&filePath);
  strFree(&mainPath);
  strFree(&buildPath);
}

int main(int argc, char *argv[]) {

  if (argc == 1) {
    printf(
        "Usage:\n - ilc run //run the project\n - ilc build //build the "
        "project\n - ilc add <library name> //add a library to the project\n");
    return 0;
  }
  dirInfo_t curDir = dirOpen(getCurrentDir());
  
  printf("OK\n");
  fflush(stdout);

  // char **archives = dirList(curDir.path ,SHOW_HIDDEN);
  if (strcmp(argv[1], "run") == 0)
    run(curDir.path);
  if (strcmp(argv[1], "build") == 0)
    build(curDir.path);
  if (strcmp(argv[1], "new") == 0) {
    if (argc >= 3)
      new(curDir.path, argv[2]);
    else
      printf("there is no name for the new project\n");
  }

  return 0;
}
