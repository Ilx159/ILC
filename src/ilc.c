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

#include "../include/ilcArray.h"
#include "../include/ilcDir.h"
#include "../include/ilcFile.h"
#include "../include/ilcString.h"
#include "../include/ilcTypes.h"
#include "../include/ilcToml.h"
#include <stdio.h>
#include <string.h>

// DEFINES

#define PATH_MAX 4096
#define FILE_NAME_MAX 512
#define LIB_MAX 512

// STRUCTS

typedef void (*argFunc)(char **);
typedef struct {
  char *name;
  argFunc func;
} cmd_t;

// GLOBAL VARIABLES

cmd_t *commands;
size_t commandCount = 0;

dirInfo_t curDir;

// FUNCTIONS

void addArg(char *str, void *func) {
  cmd_t *Tp = realloc(commands, (commandCount + 1) * sizeof(cmd_t));
  if (Tp != NULL) {
    if (commandCount != 0)
      free(commands);
    commands = Tp;
  }
  commands[commandCount] = (cmd_t){.func = func, .name = strdup(str)};
}

void checkArg(char **args) {
  for (size_t i = 0; i < commandCount; i++) {
    if (strcmp(args[1], commands[i].name) == 0)
      commands[i].func(args);
  }
}

i32 cmdAdd(fileInfo_t project_info, char lib_name[LIB_MAX]);
i32 cmdBuild(char **args);
i32 cmdRun(char **args);
i32 new(char **args);

i32 cmdBuild(char **args) {

  str_t files_path = strNew(curDir.path);

  // TOML

  str_t tomlPath = strNew(files_path.data);

  strAppend(&tomlPath, "/ilc.toml");

  toml_t tomlFile = tomlOpen(tomlPath.data);

/*

  char *tomlData = cFileRead(&tomlFile);
  str_t tomlDataStr = strNew(tomlData);

  size_t numStrings;
  str_t *path
      // strSplit(&tomlDataStr, '\n', &numStrings);
      u8 sucess1 = 0;
  size_t argsPos = 0;
  for (; argsPos <= numStrings; argsPos++) {
    if (strStartWith(&parsedContent[argsPos], "flags=")) {
      sucess1 = 1;
      break;
    }
  }
  if (!sucess1) {
    fileClose(&tomlFile);
    free(tomlData);
    strFree(&tomlDataStr);
    strFree(&toml);
    for (size_t i = 0; i < numStrings; i++) {
      strFree(&parsedContent[i]);
    }
    printf("no flags line in ilc.toml");
    return;
  }
  if (argsPos == numStrings) {
    printf("\nThere is no flags in ilc.toml!\n");
    return;
  }

  str_t flagsLine = parsedContent[argsPos];
  strStrip(&flagsLine, ' ');

  size_t numFlags = 0;
  str_t *flags = strSplit(&flagsLine, '-', &numFlags);

  cArrPop((cArr_t *)&flags[numFlags - 1]);
  numFlags--;
  for (int i = 0; i < numFlags; i++) {
    printf("%s\n", flags[i + 1].data);
    fflush(stdout);
  }

  // list src
  strAppend(&files_path, "/src");

  char **files = dirList(files_path.data, DONT_SHOW_HIDDEN);
  size_t count = 0;
  while (files[count] != NULL)
    count++;

  char **args = malloc((count + 4 + numFlags) * sizeof(char *));

  for (size_t i = 0; i < count; i++) {
    args[i + 1] = malloc(files_path.length + strlen(files[i]) + 2);
    sprintf(args[i + 1], "%s/%s", files_path.data, files[i]);
  }

  args[0] = "gcc";
  for (size_t i = 0; i < numFlags; i++) {
    cArrInsert((cArr_t *)&flags[i + 1], 0, '-');
    args[count + i + 1] = flags[i + 1].data;
  }

  args[count + numFlags + 1] = "-o";
  args[count + numFlags + 2] = "build/run";
  args[count + numFlags + 3] = NULL;

  if (execvp(args[0], args)) {
    perror("error on building");
  }

  for (size_t i = 0; i < numStrings; i++) {
    strFree(&parsedContent[i]);
  }

  */
  strFree(&files_path);
}

// AAAAAAAAAA

i32 cmdRun(char **args) {
  str_t bin_path = strNew(curDir.path);
  strAppend(&bin_path, "/build/run");
  if (execl(bin_path.data, bin_path.data, NULL) == -1) {
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
}

// MAIN

int main(int argc, char *argv[]) {

  if (argc == 1) {
    cmdHelp();
    return 0;
  }
  curDir = dirOpen(dirGetCurrent());

  checkArg(argv);

  return 0;
}
