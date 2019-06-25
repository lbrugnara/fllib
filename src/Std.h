#ifndef FL_STD_H
#define FL_STD_H
/*
 * File: Std
 *
 * Provides common data structures, functions and macros that can be used by any module and user libary. Current support:
 * 
 *  - Error handling through <struct FlError> objects and the fl_error_* functions familiy
 *  - Assertion and a exit() wrapper for quick-leave-on-error situations
 *  - Utility functions like <fl_copy> or macros like the <FLBIT> familiy intended to be used to handle modules flags
 *
 */

#include <string.h>
#include "Types.h"
#include "Error.h"

/*
 * Function: fl_copy
 *  Allocates and returns a new pointer to an element of *nbytes*
 *  bytes and copies the content of *var* in the allocated memory
 *  This is a memcpy-like function but it creates the destination
 *  pointer. If this function cannot allocate memory, it returns <NULL>
 *
 * Parameters:
 *  const void *var - A pointer to the memory to be copied in the destination pointer
 *  size_t nbytes - Number of bytes to copy from *var* to the destination pointer
 *
 * Return: 
 *  void* - A pointer to the allocated memory, or NULL if it cannot allocate memory
 *
 */
void* fl_copy(const void *var, size_t nbytes);

/*
 * Macro: fl_equals
 * ===== C =====
 *  #define fl_equals(a,b,n) (memcmp((a),(b),(n)) == 0)
 * =============
 * 
 *   Returns true if *n* bytes of *a* and *b* are equals
 * 
 *
 * Parameters:
 *   void *a - Pointer to block of memory
 *   void *b - Pointer to block of memory
 *   size_t n - Number of bytes to compare
 *
 */
#define fl_equals(a,b,n) (memcmp((a),(b),(n)) == 0)

/*
 * Macro: fl_offset_of
 * ===== C =====
 *  #define fl_offset_of(type, member) ((size_t)((char*)&((type*)0)->member-(char*)0))
 * =============
 * 
 *   Returns the offset of a given member within a struct or union
 * 
 * Parameters:
 *   type_name type - The type of the struct or union
 *   member_name member - Name of member of the struct or union
 *
 */
#define fl_offset_of(type, member) ((size_t)((char*)&((type*)0)->member-(char*)0))

#ifndef offsetof
#define offsetof fl_offset_of
#endif

/*
 * Macro: fl_container_of(ptr, type, member)
 * ===== C =====
 *  #define fl_container_of(ptr, type, member) ((type *)((char *)(ptr) - fl_offset_of(type, member)))
 * =============
 * 
 *   Returns a pointer to a struct or union based on a member of it.
 * 
 * Parameters:
 *   void* ptr - Pointer to the member
 *   type_name type - The type of the struct or union
 *   member_name member - Name of member of the struct or union
 *
 */
#define fl_container_of(ptr, type, member) ((type *)((char *)(ptr) - fl_offset_of(type, member)))

#ifndef container_of
#define container_of fl_container_of
#endif

/*
 * Macro: FLBIT
 * ===== C =====
 *  #define FLBIT(n) (1 << n)
 * =============
 * 
 *  Set the n-th bit
 *
 * Parameters:
 *  int n - Bit number
 *
 */
#define FLBIT(n)            (1 << n)

/*
 * Macro: FLBIT_ON
 * ===== C =====
 *  #define FLBIT_ON(t,b) ((t) |= (b))
 * =============
 * 
 *  Makes an OR between *t* and *b*. Intended to be use with
 *  *FLBIT* for single bits (see examples).
 *
 * Parameters:
 *  mixed t - Target value to apply OR
 *  mixed b - Source value to use in OR with *t*
 *
 * Example:
 * ===== C =====
 *  int n = 0;
 *  FLBIT_ON(n, FLBIT(2));
 *  printf("%d", n); //-> "4"
 *  FLBIT_ON(n, FLBIT(3));
 *  printf("%d", n); //-> "12"
 * 
 */
#define FLBIT_ON(t,b)       ((t) |= (b))

/*
 * Macro: FLBIT_OFF
 * ===== C =====
 *  #define FLBIT_OFF(t,b) ((t) &= ~(b))
 * =============
 * 
 *  Makes an AND between *t* and NOT(*b*). Intended to be use with
 *  *FLBIT* for single bits (see examples)
 *
 * Parameters:
 *  mixed t - Target value to apply AND
 *  mixed b - Source value to apply NOT and use in AND with *t*
 *
 * Example:
 * ===== C =====
 *  int n = 12;
 *  FLBIT_OFF(n, FLBIT(3));
 *  printf("%d", n); //-> "4"
 *  FLBIT_OFF(n, FLBIT(2));
 *  printf("%d", n); //-> "0"
 * 
 */
#define FLBIT_OFF(t,b)      ((t) &= ~(b))

/*
 * Macro: FLBIT_IS_ON
 * ===== C =====
 *  #define FLBIT_IS_ON(t,b) ((t) & (b))
 * =============
 *
 *  Intended to be used with <FLBIT>, check if bit *b* of *t* is 1
 * 
 * Parameters:
 *  mixed t - Target value to check bit *b*
 *  mixed b - Bit to check in *t*
 *
 */
#define FLBIT_IS_ON(t,b)    ((t) & (b))

/*
 * Macro: FLBIT_IS_OFF
 * ===== C =====
 *  #define FLBIT_IS_OFF(t,b) (!((t) & (b)))
 * =============
 * 
 *  Intended to be used with <FLBIT>, check if bit *b* of *t* is 0
 *
 * Parameters:
 *  mixed t - Target value to check bit *b*
 *  mixed b - Bit to check in *t*
 *
 */
#define FLBIT_IS_OFF(t,b)   (!((t) & (b)))

static inline bool fl_std_mult_wrap(long long a, long long b, long long min, long long max)
{
    if (a > 0) {  /* a is positive */
        if (b > 0) {  /* a and b are positive */
            if (a > (max / b)) {
                return true;
            }
        } else { /* a positive, b nonpositive */
            if (b < (min / a)) {
                return true;
            }
        }
    } else { /* a is nonpositive */
        if (b > 0) { /* a is nonpositive, b is positive */
            if (a < (min / b)) {
                return true;
            }
        } else { /* a and b are nonpositive */
            if ( (a != 0) && (b < (max / a))) {
                return true;
            }
        }
    }
    
    return false;
}

static inline bool fl_std_umult_wrap(unsigned long long a, unsigned long long b, unsigned long long max)
{
    return b > 0 && a > max / b;
}

#endif /* FL_STD_H */
