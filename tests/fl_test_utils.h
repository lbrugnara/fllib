#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef flm_test
#define flm_test(exp) 				\
do {								\
    if (!(exp))                     \
    {                               \
        printf(""#exp": Fail in ["__FILE__ ":" FL_LINE "].\n");  \
        exit(-1);                   \
    }                               \
    else                            \
    {                               \
        printf(""#exp": Pass.\n");  \
    }								\
} while (0)
#endif
