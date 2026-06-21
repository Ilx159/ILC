#ifndef ILCTOML_H
#define ILCTOML_H

#include "ilcTypes.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} tomlLine_t;

typedef struct {
    char *tomlChunkName;
    tomlLine_t *Line;
    size_t numLines;
} tomlChunk_t;

typedef struct {
    char *name;
    tomlChunk_t *tomlChunks;
    size_t numChunks;
} toml_t;

tomlChunk_t tomlGetChunk(toml_t *toml, char *name);
toml_t tomlOpen(const char *path);
char *tomlGet(toml_t *toml, char *chunk, char *key);
char *chunkGet(tomlChunk_t *chunk, char *key);
void tomlClose(toml_t *toml);

// #ifdef ILCTOML_IMPLEMENTATION

void _strip(char *str, char c) {
    size_t j = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] != c) {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

void _trim(char *str) {
    size_t start = 0, end = strlen(str) - 1;

    while (isspace(str[start])) {
        start++;
    }

    while (end > start && isspace(str[end])) {
        end--;
    }

    if (start > 0 || end < (strlen(str) - 1)) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }
}

tomlChunk_t tomlGetChunk(toml_t *toml, char *name) {
    for (size_t i = 0; i < toml->numChunks; i++) {
        if (strcmp(toml->tomlChunks[i].tomlChunkName, name) == 0)
            return toml->tomlChunks[i];
    }
    return (tomlChunk_t){};
}
toml_t tomlOpen(const char *path) {
    toml_t toml;
    toml.numChunks = 0;
    FILE *f = fopen(path, ("rb"));
    size_t fileSize = 0;
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    rewind(f);

    char *buffer = (char *)malloc(fileSize + 1);
    fread(buffer, fileSize, 1, f);
    size_t numLines = 1;
    buffer[fileSize] = '\0';
    size_t *startLine = (size_t *)malloc(sizeof(size_t) * fileSize / 4);
    startLine[0] = 0;
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            startLine[numLines] = i + 1;
            numLines++;
        }
    }

    toml.tomlChunks = (tomlChunk_t *)malloc(sizeof(tomlChunk_t));
    //   u8 inChunk = 0;
    char chunkNameBuffer[4096];
    char keyBuffer[4096];
    char valueBuffer[4096];

    for (size_t i = 0; i < numLines; i++) {
        char *line = &buffer[startLine[i]];
        _trim(line);
        if (line[0] == '[') {
            line[strlen(line-1)] = 0; 
            toml.tomlChunks[toml.numChunks].tomlChunkName =
                strdup(&chunkNameBuffer[1]);
            toml.numChunks++;
            toml.tomlChunks = (tomlChunk_t *)realloc(
                toml.tomlChunks, (toml.numChunks + 1) * sizeof(tomlChunk_t));
            toml.tomlChunks[toml.numChunks - 1].Line =
                (tomlLine_t *)malloc(sizeof(tomlLine_t));
            toml.tomlChunks[toml.numChunks - 1].numLines = 0;
        } else if (strchr(line, '=') != NULL) {
            toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .key = strdup(keyBuffer);
            toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .value = strdup(valueBuffer);
            toml.tomlChunks[toml.numChunks - 1].numLines++;
            toml.tomlChunks[toml.numChunks - 1].Line = (tomlLine_t *)realloc(
                toml.tomlChunks[toml.numChunks - 1].Line,
                sizeof(tomlLine_t) *
                    (toml.tomlChunks[toml.numChunks - 1].numLines + 1));
        }
    }
    fclose(f);
    free(buffer);
    return toml;
}
char *tomlGet(toml_t *toml, char *chunk, char *key) {}
char *chunkGet(tomlChunk_t *chunk, char *key) {}
void tomlClose(toml_t *toml) {}

// endif
#endif
