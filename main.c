#include <stdio.h>
#include <stdlib.h>
#include "include/array.h"

int main(void) {

    u32Arr_t vec = u32ArrNew((u32*)NULL, 0);
    for(u32 i = 0; i < 10; ++i){
        u32ArrPush(&vec, i);
    }
    u32ArrReverse(&vec);

    for(size_t i = 0; i < vec.length; ++i){
        printf("%u ", vec.data[i]);
    }
    printf("\n");
    u32ArrFree(&vec);
    return 0;

}
