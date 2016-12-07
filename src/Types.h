#ifndef FL_TYPES_H
#define FL_TYPES_H

#include <string.h>

typedef unsigned char   FlByte;
typedef char*           FlCstr;
typedef void*           FlPointer;
typedef unsigned int    FlUint;

#ifndef NULL
    #define NULL (void*)0
#endif

#define FL_EOS '\0'

#ifdef HAVE_STDBOOL_H
    #include <stdbool.h>
#else
    #ifndef HAVE__BOOL
        #ifdef __cplusplus
            typedef bool _Bool;
        #else
            #define _Bool signed char
        #endif
    #endif
    #define bool _Bool
    #define FlBool bool
    #define false 0
    #define true 1
    #ifndef TRUE
        #define TRUE true
    #endif
    #ifndef FALSE
        #define FALSE false
    #endif
    #define __bool_true_false_are_defined 1
#endif

#define FL_PRINT_VAR(x) #x
#define FL_STRINGIFY(x) FL_PRINT_VAR(x)
#define FL_LINE FL_STRINGIFY(__LINE__)

#endif
