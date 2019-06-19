#ifndef FL_TYPES_H
#define FL_TYPES_H

/*
 * Type: FlByte
 * ===== C =====
 *  typedef unsigned char FlByte;
 * =============
 * 
 *   Represents a single byte. Preferred over *unsigned char* for readability, but it is not a must.
 *
 */
typedef unsigned char FlByte;


/*
 * define: FlString
 * ===== C =====
 *  #define FlString char*
 * =============
 * 
 *   Represents an UTF8 string
 * 
 */
#define FlString char*

#ifndef NULL
    /*
    * define: NULL
    * ===== C =====
    *   #define NULL (void*)0
    * =============
    * 
    *   Null pointer
    * 
    */
    #define NULL (void*)0
#endif

/*
 * define: FL_EOS
 * ===== C =====
 *  #define FL_EOS '\0'
 * =============
 * 
 *   End of string
 * 
 */
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
