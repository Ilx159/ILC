#ifndef ILCMATH_H

#include "ilcTypes.h"
#define SIN_LENGTH 360
#define PI (355.0f/113.0f)

void ilcMathInit(void);
float ilcSin(i32 a);
float ilcCos(i32 a);

static inline f64 clampf(f64 x, f64 min, f64 max){
  if(x < min) return min;
  if(x > max) return max;
  return x;
}

static inline i32 clampi(i32 x, i32 min, i32 max){
  if(x < min) return min;
  if(x > max) return max;
  return x;
}

static inline f64 absf(f64 x){
  return x < 0 ? -x : x;
}

void interpolate(vec2i vec1, vec2i vec2, i32 *x, i32 size);
 

#ifdef ILCMATH_IMPLEMENTATION


static float sin_lut[SIN_LENGTH];

void math_init(void){
  for(int i = 0; i < SIN_LENGTH; i++)
  sin_lut[i] = sin(((float)i*PI)/180);
}

float ilcSin(u16 a){
  a = a % 360;
  return sin_lut[a];
}

float ilcCos(u16 a){
  a += 90;
  a = a % 360;  
  return sin_lut[a];
}

void interpolate(vec2i vec1, vec2i vec2, i32 *x, i32 size){
  if (size < vec2.y - vec1.y) return;
  if (vec1.y == vec2.y) return;
  float slope = ((float)(vec2.x - vec1.x)/(float)(vec2.y - vec1.y));
  for(u16 i = vec1.y, j = 0; i < vec2.y; i++, j++){
    x[j] = (u16)roundf((float)(i - vec1.y)*(slope) + (float)vec1.x); 
  }
}

#endif
#endif
