#ifndef ILCTYPES_H
#define ILCTYPES_H

#include <stdint.h>
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint64_t u64;
typedef int16_t i16;
typedef int32_t i32;
typedef int8_t i8;
typedef int64_t i64;
typedef float f32;
typedef double f64;

typedef struct {
  int x, y;
} vec2i;

typedef struct {
  float x, y;
}vec2f;

typedef struct {
  float x, y, z;
} vec3f;

typedef struct {
  int x, y, z;
} vec3i;

typedef struct {
  int w, x, y, z;
} vec4i;

typedef struct {
  float w, x, y, z;
} vec4f;

#endif
