#ifndef FL_WINEX_H
#define FL_WINEX_H

/* -------------------------------------------------------------
 * Win32 SEH ({see: os/Windows.h})
 * -------------------------------------------------------------
 * Add support for Windows SEH through an interface similar to
 * signal.h
 */
#include <windows.h>

/* -------------------------------------------------------------
 * WIN32 SEH
 * -------------------------------------------------------------
 *  EXCEPTION_ACCESS_VIOLATION
 *  EXCEPTION_ARRAY_BOUNDS_EXCEEDED
 *  EXCEPTION_BREAKPOINT
 *  EXCEPTION_DATATYPE_MISALIGNMENT
 *  EXCEPTION_FLT_DENORMAL_OPERAND
 *  EXCEPTION_FLT_DIVIDE_BY_ZERO
 *  EXCEPTION_FLT_INEXACT_RESULT
 *  EXCEPTION_FLT_INVALID_OPERATION
 *  EXCEPTION_FLT_OVERFLOW
 *  EXCEPTION_FLT_STACK_CHECK
 *  EXCEPTION_FLT_UNDERFLOW
 *  EXCEPTION_ILLEGAL_INSTRUCTION
 *  EXCEPTION_IN_PAGE_ERROR
 *  EXCEPTION_INT_DIVIDE_BY_ZERO
 *  EXCEPTION_INT_OVERFLOW
 *  EXCEPTION_INVALID_DISPOSITION
 *  EXCEPTION_NONCONTINUABLE_EXCEPTION
 *  EXCEPTION_PRIV_INSTRUCTION
 *  EXCEPTION_SINGLE_STEP
 *  EXCEPTION_STACK_OVERFLOW
 *  STATUS_WAIT_0
 *  STATUS_ABANDONED_WAIT_0
 *  STATUS_USER_APC
 *  STATUS_TIMEOUT
 *  STATUS_PENDING
 *  STATUS_SEGMENT_NOTIFICATION
 *  STATUS_GUARD_PAGE_VIOLATION
 *  STATUS_NO_MEMORY
 *  STATUS_CONTROL_C_EXIT
 */

 /* -------------------------------------------------------------
 * WIN32 Console Control
 * -------------------------------------------------------------
 *  CTRL_C_EVENT
 *  CTRL_CLOSE_EVENT
 *  CTRL_BREAK_EVENT
 *  CTRL_LOGOFF_EVENT
 *  CTRL_SHUTDOWN_EVENT
 */

typedef LONG WINAPI(*FlWinExceptionHandler)(EXCEPTION_POINTERS * ExceptionInfo);
typedef BOOL (*FlWinCmdControlHandler)( DWORD ctrlType );

 /* -------------------------------------------------------------
  * {function: fl_winex_handler_set}
  * -------------------------------------------------------------
  * Register an exception handler for the specified exception type 
  * -------------------------------------------------------------
  * {param: DWORD exceptionCode} Exception type
  * {param: FlWinExceptionHandler handler} Handler function
  * -------------------------------------------------------------
  * {return: FlWinExceptionHandler} Previous exception handler
  * -------------------------------------------------------------
  */
FlWinExceptionHandler fl_winex_handler_set(DWORD exceptionCode, FlWinExceptionHandler handler);

/* -------------------------------------------------------------
 * {function: fl_winex_global_handler_set}
 * -------------------------------------------------------------
 * Registers a global exception handler to be called every time
 * an SE occurs
 * -------------------------------------------------------------
 * {return: FlWinExceptionHandler} Previous exception handler
 * -------------------------------------------------------------
 */
FlWinExceptionHandler fl_winex_global_handler_set(FlWinExceptionHandler handler);

/* -------------------------------------------------------------
 * {function: fl_winex_message_get}
 * -------------------------------------------------------------
 * Copies on {datamsg} a brief description of the exception
 * represented by code {exceptionCode}
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_winex_message_get(DWORD exceptionCode, char *destmsg);

/* -------------------------------------------------------------
 * {function: fl_wincmd_control_handler_set}
 * -------------------------------------------------------------
 * Registers a control handler to be called when a control input
 * of type {ctrl} occurs
 * -------------------------------------------------------------
 * {param: DWORD ctrl} Specific control handler (CTRL_C_EVENT, CTRL_CLOSE_EVENT, CTRL_BREAK_EVENT, CTRL_LOGOFF_EVENT or CTRL_SHUTDOWN_EVENT)
 * {param: FlWinCmdControlHandler handler} Handler function
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_wincmd_control_handler_set(DWORD ctrl, FlWinCmdControlHandler handler);

#endif /* FL_WINEX_H */
