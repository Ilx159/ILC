#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
  char * path = getcwd(NULL, 0);
  printf("%s\n", path);
  free(path);
  return 0;
}
