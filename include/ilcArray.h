#ifndef ILCARRAY_H 
  #define ILCARRAY_H

  #include <string.h>
  #include <stdlib.h>
  #include <stdint.h>
  #include "ilcTypes.h"
  #define XARR_TYPES\
  X(u8, uint8_t)\
  X(u16, uint16_t)\
  X(u32, uint32_t)\
  X(u64, uint64_t)\
  X(i8, int8_t)\
  X(i16, int16_t)\
  X(i32, int32_t)\
  X(i64, int64_t)\
  X(f32, float)\
  X(f64, double)\
  X(c, char)

#define X(name, type)\
  typedef struct{\
    type *data;\
    size_t length;\
    size_t capacity;\
  } name##Arr_t;
XARR_TYPES
#undef X

#define X(name, type)\
  static inline void name##ArrFree(name##Arr_t *arr);\
  static inline void name##ArrClear(name##Arr_t *arr);\
  static inline void name##ArrAddCap(name##Arr_t *arr, size_t new_capacity);\
  static inline name##Arr_t name##ArrNew(type *data, size_t length);\
  static inline name##Arr_t name##ArrCopy(name##Arr_t arr);\
  static inline void name##ArrAssign(name##Arr_t *arr, type *data, size_t length);\
  static inline type name##ArrGet(name##Arr_t *arr, size_t index);\
  static inline void name##ArrPush(name##Arr_t *arr, type value);\
  static inline void name##ArrPop(name##Arr_t *arr);\
  static inline void name##ArrSet(name##Arr_t *arr, size_t index, type value);\
  static inline void name##ArrInsert(name##Arr_t *arr, size_t index, type value);\
  static inline void name##ArrRemove(name##Arr_t *arr, size_t index);\
  static inline u8 name##ArrContains(name##Arr_t *arr, type value);\
  static inline size_t name##ArrFind(name##Arr_t *arr, type value);\
  static inline void name##ArrReverse(name##Arr_t *arr);\
  static inline void name##ArrFill(name##Arr_t *arr, type value);\
  static inline void name##ArrMap(name##Arr_t *arr, type (*func)(type));\
  static inline void name##ArrSort(name##Arr_t *arr);
XARR_TYPES
#undef X

#ifdef ILCARRAY_IMPLEMENTATION
#define X(name, type)\
  static inline void name##ArrFree(name##Arr_t *arr){\
      free(arr->data);\
      arr->data = NULL;\
      arr->length = 0;\
      arr->capacity = 0;\
  }\
  static inline void name##ArrClear(name##Arr_t *arr){\
      arr->length = 0;\
  }\
  static inline void name##ArrAddCap(name##Arr_t *arr, size_t new_capacity){\
      size_t item_size = sizeof(arr->data[0]);\
      type *tmp = realloc(arr->data, new_capacity * item_size);\
      if(tmp){\
        arr->data = tmp;\
        arr->capacity = new_capacity;\
      }\
  }\
  static inline name##Arr_t name##ArrNew(type *data, size_t length){\
    name##Arr_t arr;\
    arr.length = length;\
    if(length == 0 || data == NULL){\
        arr.capacity = 1;\
        arr.data = malloc(arr.capacity * sizeof(type));\
    }\
    else{\
        arr.capacity = length;\
        arr.data = malloc(arr.capacity * sizeof(type));\
        memcpy(arr.data, data, length * sizeof(type));\
    }\
    return arr;\
  }\
  static inline name##Arr_t name##ArrCopy(name##Arr_t arr){\
    name##Arr_t cpArr;\
    cpArr.length = arr.length;\
    cpArr.data = malloc(arr.capacity * sizeof(type));\
    memcpy(cpArr.data, arr.data, arr.length * sizeof(type));\
    cpArr.capacity = arr.capacity;\
    return cpArr;\
  }\
  static inline void name##ArrAssign(name##Arr_t *arr, type *data, size_t length){\
      if(arr->capacity < length){\
          arr->capacity = length;\
          arr->data = realloc(arr->data, arr->capacity * sizeof(type));\
      }\
      memcpy(arr->data, data, length * sizeof(type));\
      arr->length = length;\
  }\
  static inline type name##ArrGet(name##Arr_t *arr, size_t index){\
      if(index >= arr->length){\
          return (type)0;\
      }\
      return arr->data[index];\
  }\
  static inline void name##ArrPush(name##Arr_t *arr, type value){\
      if(arr->length >= arr->capacity){\
          size_t new_capacity = arr->capacity == 0 ? 1 : arr->capacity * 2;\
          name##ArrAddCap(arr, new_capacity);\
      }\
      arr->data[arr->length++] = value;\
  }\
  static inline void name##ArrPop(name##Arr_t *arr){\
      if(arr->length > 0){\
          arr->length--;\
          arr->data[arr->length] = (type)0;\
      }\
  }\
  static inline void name##ArrSet(name##Arr_t *arr, size_t index, type value){\
      if(index < arr->length){\
          arr->data[index] = value;\
      }\
      else{\
      return;\
      }\
  }\
  static inline void name##ArrInsert(name##Arr_t *arr, size_t index, type value){\
      if(index > arr->length){\
          return;\
      }\
      if(arr->length + 1 > arr->capacity){\
          size_t new_capacity = arr->capacity == 0 ? 1 : arr->capacity * 2;\
          name##ArrAddCap(arr, new_capacity);\
      }\
      memmove(&arr->data[index + 1], &arr->data[index], (arr->length - index) * sizeof(type));\
      arr->length++;\
      arr->data[index] = value;\
      \
  }\
  static inline void name##ArrRemove(name##Arr_t *arr, size_t index){\
      if(index >= arr->length){\
          return;\
      }\
      memmove(&arr->data[index], &arr->data[index + 1], (arr->length - index - 1) * sizeof(type));\
      arr->length--;\
      arr->data[arr->length] = (type)0;\
  }\
  static inline u8 name##ArrContains(name##Arr_t *arr, type value){\
      for(size_t i = 0; i < arr->length; ++i){\
          if(arr->data[i] == value){\
              return 1;\
          }\
      }\
      return 0;\
  }\
  static inline size_t name##ArrFind(name##Arr_t *arr, type value){\
      for(size_t i = 0; i < arr->length; ++i){\
          if(arr->data[i] == value){\
              return i;\
          }\
      }\
      return (size_t)-1;\
  }\
  static inline void name##ArrReverse(name##Arr_t *arr){\
      for(size_t i = 0; i < arr->length / 2; ++i){\
          type temp = arr->data[i];\
          arr->data[i] = arr->data[arr->length - i - 1];\
          arr->data[arr->length - i - 1] = temp;\
      }\
    }\
  static inline void name##ArrFill(name##Arr_t *arr, type value){\
      for(size_t i = 0; i < arr->length; ++i){\
          arr->data[i] = value;\
      }\
  }\
  static inline void name##ArrMap(name##Arr_t *arr, type (*func)(type)){\
      for(size_t i = 0; i < arr->length; ++i){\
          arr->data[i] = func(arr->data[i]);\
      }\
  }\
  static inline int _##name##ArrCompare(const void *a, const void *b){\
    return (*(type *)a > *(type *)b) - (*(type *)a < *(type *)b);\
  }\
  static inline void name##ArrSort(name##Arr_t *arr){\
      qsort(arr->data, arr->length, sizeof(type), _##name##ArrCompare);\
  }
XARR_TYPES
#undef X
#endif /* ARRAY_IMPLEMENTATION */
#endif
