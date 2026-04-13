#ifndef ILCSTRING_H
  #define ILCSTRING_H
  
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>
  #include "ilcTypes.h"

  typedef struct {
    char *data;
    size_t length;
    size_t capacity;
  } str_t;
  
  str_t strNew(char *str);
  str_t strCopy(str_t str);
  void strFree(str_t *str);

  void strAppend(str_t *str1, char *str2);
  str_t *strSplit(str_t *str, char delimiter, size_t *count); //last in the char will be NULL for counting
  void strTrim(str_t *str);
  size_t strContains(str_t *str1, char *str2);
  u8 strStartWith(str_t *str, char *dest);
  size_t *strFind(str_t *str, char c);

#ifdef ILCSTRING_IMPLEMENTATION
  
  str_t strNew(char *str){
    str_t STR;
    STR.length = strlen(str);
    STR.capacity = STR.length + 1;
    STR.data = malloc(STR.capacity);
    memcpy(STR.data, str, STR.capacity);
    STR.data[STR.length] = '\0';
    return STR;
  }

  str_t strCopy(str_t str){
    str_t copy = str;
    copy.data = malloc(str.capacity);
    memcpy(copy.data, str.data, str.capacity);
    return copy;
  }
  
  void strFree(str_t *str){
    free(str->data);
    str->data = NULL;
    str->capacity = 0;
    str->length = 0;
  }

  void strAppend(str_t *str1, char *str2){
    size_t fullSize = str1->length + strlen(str2);
    str1->data = realloc(str1->data, fullSize + 1);
    str1->length = fullSize;
    str1->capacity = fullSize + 1;
    strcat(str1->data, str2);
    str1->data[fullSize] = '\0';
  }

  str_t *strSplit(str_t *str, char delimiter, size_t *count){ //last in the char will be NULL for counting
   
    char *temp = strdup(str->data);
    size_t positions[str->length/2];
    positions[0] = 0;
    size_t j = 1;
    for(size_t i = 0; i < str->length; i++){
      if(str->data[i] == delimiter){
        positions[j] = i;
        temp[i] = '\0';
        j++;
      } 
    }
    str_t *strs = (str_t *)malloc((sizeof(str_t) * j) + 1);
    for(size_t i = 0; i < j; i++){
      strs[i] = strNew(&temp[positions[i]]);
    }
    free(temp);
    *count = j;
    return strs;
  }

  void strTrim(str_t *str){
    size_t start = 0, end = strlen(str->data) - 1;

    while (isspace(str->data[start])) {
      start++;
    }

    while (end > start && isspace(str->data[end])) {
      end--;
    }

    if(start > 0 || end < (strlen(str->data) - 1)) {
      memmove(str->data, str->data + start, end - start + 1);
      str->data[end - start + 1] = '\0';
    }
    str->length = end-start;
  }

  size_t strContains(str_t *str1, char *str2){
    size_t position = 0;
    size_t lenghtStr2 = strlen(str2);
    for(;str1->data[position] != '\0'; position++){
      if(str1->data[position] == str2[0]){
        if(strncmp(str2, &str1->data[position], strlen(str2)) == 0) return position;
      }
    }
    return (size_t)-1;
  }

  u8 strStartWith(str_t *str, char *dest){ //supose str->data has a longer lenght than dest;
    return strncmp(dest, str->data, strlen(dest)) == 0;
  }
  
  size_t *strFind(str_t *str, char c){//returns an array of where is the char, and the last pos will be 0 to know when ends
    size_t *pos = (size_t *)malloc((str->length+1)*sizeof(size_t));
    size_t arrpos = 0;

    for(size_t i = 0; i < str->length; i++){
      if(str->data[i] == c){
        pos[arrpos] = i;
        arrpos++;
      }
    }
    pos[arrpos] = 0;
    return pos;
  }
  //#endif
#endif
#endif
