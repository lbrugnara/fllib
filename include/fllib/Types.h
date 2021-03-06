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

#endif
