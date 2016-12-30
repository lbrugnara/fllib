#ifndef FL_STD_H
#define FL_STD_H
/* =============================================================
 * {module: Std}
 * =============================================================
 * Provides common data structures, functions and macros that
 * can be used by any module and user libary.
 * Current support:
 *  - Error handling through {FlError} objects and the {fl_error_*} functions familiy
 *  - Assertion and a exit() wrapper for quick-leave-on-error situations
 *  - Utility functions like {fl_copy} or macros like the {FLBIT} familiy intended to be used to handle modules flags
 * -------------------------------------------------------------
 */

#include "Types.h"
#include "Error.h"

/* -------------------------------------------------------------
 * {function: fl_copy}
 * -------------------------------------------------------------
 * Allocates and returns a new pointer to an element of {nbytes}
 * bytes and copies the content of {var} in the allocated memory
 * This is a memcpy-like function but it creates the destination
 * pointer.
 * -------------------------------------------------------------
 * {param: const FlPointer var} A pointer to the memory to be copied in the destination pointer
 * {param: size_t nbytes} Number of bytes to copy from {var} in destination pointer
 * -------------------------------------------------------------
 * {return: FlPointer} A pointer to the allocated memory
 * -------------------------------------------------------------
 */
FlPointer fl_copy(const FlPointer var, size_t nbytes);

/* -------------------------------------------------------------
 * {macro: FLBIT}
 * -------------------------------------------------------------
 * Set an n-th bit
 * -------------------------------------------------------------
 * {param: int n} Bit number
 * -------------------------------------------------------------
 */
#define FLBIT(n)            (1 << n)

/* -------------------------------------------------------------
 * {macro: FLBIT_ON}
 * -------------------------------------------------------------
 * Makes an OR between {t} and {b}. Intended to be use with
 * {FLBIT} for single bits (see examples).
 * -------------------------------------------------------------
 * {param: mixed t} Target value to apply OR
 * {param: mixed b} Source value to use in OR with {t}
 * -------------------------------------------------------------
 * {example}
 *  int n = 0;
 *  FLBIT_ON(n, FLBIT(2));
 *  printf("%d", n); //-> "4"
 *  FLBIT_ON(n, FLBIT(3));
 *  printf("%d", n); //-> "12"
 */
#define FLBIT_ON(t,b)       ((t) |= (b))

/* -------------------------------------------------------------
 * {macro: FLBIT_OFF}
 * -------------------------------------------------------------
 * Makes an AND between {t} and NOT({b}). Intended to be use with
 * {FLBIT} for single bits (see examples)
 * -------------------------------------------------------------
 * {param: mixed t} Target value to apply AND
 * {param: mixed b} Source value to apply NOT and use in AND with {t}
 * -------------------------------------------------------------
 * {example}
 *  int n = 12;
 *  FLBIT_OFF(n, FLBIT(3));
 *  printf("%d", n); //-> "4"
 *  FLBIT_OFF(n, FLBIT(2));
 *  printf("%d", n); //-> "0"
 */
#define FLBIT_OFF(t,b)      ((t) &= ~(b))

/* -------------------------------------------------------------
 * {macro: FLBIT_IS_ON}
 * -------------------------------------------------------------
 * Intended to be used with {FLBIT}, check if bit {b} of {t} is 1
 * -------------------------------------------------------------
 * {param: mixed t} Target value to check bit {b}
 * {param: mixed b} Bit to check in {t}
 * -------------------------------------------------------------
 */
#define FLBIT_IS_ON(t,b)    ((t) & (b))

/* -------------------------------------------------------------
 * {macro: FLBIT_IS_OFF}
 * -------------------------------------------------------------
 * Intended to be used with {FLBIT}, check if bit {b} of {t} is 0
 * -------------------------------------------------------------
 * {param: mixed t} Target value to check bit {b}
 * {param: mixed b} Bit to check in {t}
 * -------------------------------------------------------------
 */
#define FLBIT_IS_OFF(t,b)   (!((t) & (b)))

#endif /* FL_STD_H */
