#ifndef FL_WIN_H
#define FL_WIN_H

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
typedef LONG WINAPI(*FlWinExceptionHandler)(EXCEPTION_POINTERS * ExceptionInfo);
FlWinExceptionHandler fl_winex_handler_set(DWORD exceptionCode, FlWinExceptionHandler handler);
FlWinExceptionHandler fl_winex_global_handler_set(FlWinExceptionHandler handler);
void fl_winex_message_get(DWORD exceptionCode, char *destmsg);

/* -------------------------------------------------------------
 * WIN32 Console Control
 * -------------------------------------------------------------
 *  CTRL_C_EVENT
 *  CTRL_CLOSE_EVENT
 *  CTRL_BREAK_EVENT
 *  CTRL_LOGOFF_EVENT
 *  CTRL_SHUTDOWN_EVENT
 */
typedef BOOL (*FlWinCmdControlHandler)( DWORD ctrlType );
void fl_wincmd_control_handler_set(DWORD ctrl, FlWinCmdControlHandler handler);

#endif