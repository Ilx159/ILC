#ifndef ILCTEST_H
#define ILCTEST_H

#include <stdio.h>

#define ILC_TEST_RUN(test)                                                     \
  do {                                                                         \
    printf("Running %s... ", #test);                                           \
    if (test()) {                                                              \
      printf("\033[0;32mOK\033[0m\n");                                    \
    } else {                                                                   \
      printf("\033[0;31mFAIL\033[0m\n");                                     \
    }                                                                          \
  } while (0)

#endif
