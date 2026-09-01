/* O que é o ILC?
 *
 * é uma ferramenta que me ajuda a criar novos projetos, permitindo
 * configurações de build (flags) e é possivel rodar código em C por ele.
 *
 * permite eu adicionar e remover libs próprias (ecossistema ilc), assim
 * permitindo uma construção mais rápida do código.
 *
 */

#define ILCDIR_IMPLEMENTATION
#define ILCARRAY_IMPLEMENTATION
#define ILCFILE_IMPLEMENTATION
#define ILCSTRING_IMPLEMENTATION
#define ILCTOML_IMPLEMENTATION

#include "../include/ilcArray.h"
#include "../include/ilcDir.h"
#include "../include/ilcFile.h"
#include "../include/ilcString.h"
#include "../include/ilcToml.h"
#include "../include/ilcTypes.h"
#include <stdio.h>
#include <string.h>

// DEFINES

#define PATH_MAX 4096
#define FILE_NAME_MAX 512
#define LIB_MAX 512

// STRUCTS

typedef u32 (*argFunc)(char **);
typedef struct {
  char *name;
  argFunc func;
} cmd_t;

// GLOBAL VARIABLES

cmd_t *commands;
size_t commandCount = 0;

dirInfo_t curDir;

// FUNCTIONS

i32 cmdHelp() {
  printf("Usage:\n - ilc run //run the project\n - ilc build //build the "
         "project\n - ilc add <library name> //add a library to the project\n");
  return 0;
}

void addArg(char *str, void *func) {
  cmd_t *Tp = realloc(commands, (commandCount + 1) * sizeof(cmd_t));
  if (Tp != NULL) {
    commands = Tp;
  }
  commands[commandCount] = (cmd_t){.func = func, .name = strdup(str)};
  commandCount++;
}

void checkArg(char **args) {
  for (size_t i = 0; i < commandCount; i++) {
    if (strcmp(args[1], commands[i].name) == 0)
      if (commands[i].func(args) != 0) cmdHelp();
  }
}

i32 cmdAdd(fileInfo_t project_info, char lib_name[LIB_MAX]);
i32 cmdBuild(char **args);
i32 cmdRun(char **args);
i32 cmdNew(char **args);

i32 cmdBuild(char **args) {

  str_t filesPath = strNew(curDir.path);

  // TOML

  str_t tomlPath = strNew(filesPath.data);
  strAppend(&tomlPath,
            "/ilc.toml"); // if return -1 or anything else, didn't find ilc.toml

  toml_t tomlFile = tomlOpen(tomlPath.data);

  str_t argsFull = strNew(tomlGet(&tomlFile, "project", "flags"));

  size_t ArgsNum = 0;
  str_t *Args = strSplit(&argsFull, ' ', &ArgsNum);

  // SRC

  str_t srcPath = strNew(filesPath.data);
  strAppend(&srcPath, "/src");

  pathsList_t srcFiles = dirListRecursive(srcPath.data, DONT_SHOW_HIDDEN);

  char **binArgs = malloc((4 + srcFiles.numItens + ArgsNum) * sizeof(char *));

  binArgs[0] = "gcc";


  size_t count = 0;
for (size_t i = 0; i < srcFiles.numItens; i++) { //tem que verificar se é .c
    binArgs[count + 1] = srcFiles.itensPaths[i];
    count++;
}

  for (size_t i = 0; i < ArgsNum; i++) {
    binArgs[count + i + 1] = Args[i].data;
  }

  binArgs[count + ArgsNum + 1] = "-o";
  binArgs[count + ArgsNum + 2] = "build/run";
  binArgs[count + ArgsNum + 3] = NULL;

  if (execvp(binArgs[0], binArgs)) {
    perror("error on building");
  }

  for (size_t i = 0; i < ArgsNum; i++) {
    strFree(&Args[i]);
  }

  free(binArgs);
  freePathsList(&srcFiles);
  tomlClose(&tomlFile);
  strFree(&srcPath);
  strFree(&argsFull);
  strFree(&filesPath);

  return 0;
}

i32 cmdRun(char **args) {
  str_t bin_path = strNew(curDir.path);
  strAppend(&bin_path, "/build/run");
  if (execvp(bin_path.data, &args[1]) == -1) {
    perror("Error");
    strFree(&bin_path);
    return -1;
  }
  strFree(&bin_path);
  return 0;
}

i32 cmdNew(char **args) {

  char *name = strdup(args[2]);

  str_t fullpath = strNew(curDir.path);
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
    return -1;
  }

  dirCreate(fullpath.data);
  dirCreate(includePath.data);
  dirCreate(srcPath.data);
  dirCreate(buildPath.data);

  // cria ilc.toml
  fileInfo_t toml = fileOpen(filePath.data, "wb");
  char content[256];
  sprintf(content,
          "[project]\nname=\"%s\"\nversion=\"0.1.0\"\nflags="
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

  return 0;
}


// MAIN

int main(int argc, char *argv[]) {

  if (argc == 1) {
    cmdHelp();
    return 0;
  }
  curDir = dirOpen(dirGetCurrent());
  addArg("build", cmdBuild);
  addArg("run", cmdRun);
  addArg("new", cmdNew);
  addArg("help", cmdHelp);

  checkArg(argv);

  return 0;
}
