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
void freeChunk(tomlChunk_t *chunk);
char *chunkGet(tomlChunk_t *chunk, char *key);
void tomlClose(toml_t *toml);

#ifdef ILCTOML_IMPLEMENTATION

void _trim(char *str) {
    if (strlen(str) == 0)
        return;
    size_t start = 0, end = strlen(str) - 1;

    while (isspace((unsigned char)str[start])) {
        start++;
    }

    while (end > start && isspace((unsigned char)str[end])) {
        end--;
    }

    if (start > 0 || end < (strlen(str) - 1)) {
        memmove(str, str + start, end - start);
        str[end - start] = '\0';
    }
}

tomlChunk_t tomlGetChunk(toml_t *toml, char *name) {
    for (size_t i = 0; i < toml->numChunks; i++) {
        if (strcmp(toml->tomlChunks[i].tomlChunkName, name) == 0)
            return toml->tomlChunks[i];
    }
    return (tomlChunk_t){};
}

static char *parseValue(char *value)
{
    while (*value == ' ' || *value == '\t')
        value++;

    size_t len = strlen(value);

    if (len >= 2 && value[0] == '"' && value[len - 1] == '"') {
        value[len - 1] = '\0';
        value++;
    }

    return strdup(value);
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
    if (buffer == NULL)
        return (toml_t){0};
    if (fread(buffer, fileSize, 1, f) == 0) {
        free(buffer);
        return (toml_t){0};
    }
    size_t numLines = 1;
    buffer[fileSize] = '\0';
    size_t *startLine = (size_t *)malloc(sizeof(size_t) * (fileSize + 1));
    if (startLine == NULL) {
        free(buffer);
        return (toml_t){0};
    }
    startLine[0] = 0;
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            startLine[numLines] = i + 1;
            numLines++;
        }
    }

    toml.tomlChunks = NULL;

    for (size_t i = 0; i < numLines; i++) {
        char *line = &buffer[startLine[i]];
        _trim(line);

        if (line[0] == '[') {
            line[strlen(line) - 1] = 0;
            toml.numChunks++;
            tomlChunk_t *newChunks = (tomlChunk_t *)realloc(
                toml.tomlChunks, toml.numChunks * sizeof(tomlChunk_t));
            if (newChunks == NULL)
                return (toml_t){0};
            toml.tomlChunks = newChunks;
            toml.tomlChunks[toml.numChunks - 1].tomlChunkName = strdup(&line[1]);
            toml.tomlChunks[toml.numChunks - 1].Line =
                (tomlLine_t *)malloc(sizeof(tomlLine_t));
            toml.tomlChunks[toml.numChunks - 1].numLines = 0;

        } else if ((strchr(line, '=') != NULL) && toml.numChunks != 0) {
            char *Tpointer = strchr(line, '=');
            Tpointer[0] = 0;
            toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .key = strdup(&line[0]);
            _trim(toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .key);
            toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .value = parseValue(Tpointer + 1);
            toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .value[strlen(toml.tomlChunks[toml.numChunks - 1]
                .Line[toml.tomlChunks[toml.numChunks - 1].numLines]
                .value)+1] = 0;
            toml.tomlChunks[toml.numChunks - 1].numLines++;
            tomlLine_t *newLines = (tomlLine_t *)realloc(
                toml.tomlChunks[toml.numChunks - 1].Line,
                sizeof(tomlLine_t) *
                    (toml.tomlChunks[toml.numChunks - 1].numLines + 1));
            if (newLines == NULL)
                return (toml_t){0};
            toml.tomlChunks[toml.numChunks - 1].Line = newLines;
        }
    }
    fclose(f);
    free(buffer);
    free(startLine);
    return toml;
}

char *chunkGet(tomlChunk_t *chunk, char *key) {
    for (size_t i = 0; i < chunk->numLines; i++) {
        if (strcmp(chunk->Line[i].key, key) == 0)
            return chunk->Line[i].value;
    }
    return 0;
}

char *tomlGet(toml_t *toml, char *chunk, char *key) {
    tomlChunk_t Tchunk = tomlGetChunk(toml, chunk);
    char *content;
    content = chunkGet(&Tchunk, key);
    return content;
}

void freeChunk(tomlChunk_t *chunk) {
    for (size_t j = 0; j < chunk->numLines; j++) {
        free(chunk->Line[j].key);
        free(chunk->Line[j].value);
    }
    free(chunk->Line);
}
void tomlClose(toml_t *toml) {
    for (size_t i = 0; i < toml->numChunks; i++) {
        freeChunk(&toml->tomlChunks[i]);
    }
    free(toml->tomlChunks);
}

#endif
#endif
