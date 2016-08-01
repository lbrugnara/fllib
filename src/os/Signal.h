#ifndef FL_SIGNAL_H
#define FL_SGINAL_H

/* -------------------------------------------------------------
 * C99 Signals
 * -------------------------------------------------------------
 * SIGABRT
 * SIGFPE
 * SIGILL
 * SIGINT
 * SIGSEGV
 * SIGTERM
 */
#include <signal.h>

/* -------------------------------------------------------------
 * {datatype: void (*)(int) FlSignalHandler}
 * -------------------------------------------------------------
 * Signals handler function. Used to call signal(3)
 * -------------------------------------------------------------
 */
typedef void(*FlSignalHandler)(int sig);

/* -------------------------------------------------------------
 * {function: fl_signal_handler_set}
 * -------------------------------------------------------------
 * Set the FlSignalHandler for a specific signal
 * -------------------------------------------------------------
 * {param: int sig} Signal number
 * {param: FlSignalHandler handler} Signal handler function
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_signal_handler_set(int sig, FlSignalHandler handler);

/* -------------------------------------------------------------
 * {function: fl_signal_global_handler_set}
 * -------------------------------------------------------------
 * Set an FlSignalHandler for all the signals defined in
 * the C standard
 * -------------------------------------------------------------
 * {param: FlSignalHandler handler} Signal handler function
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_signal_global_handler_set(FlSignalHandler handler);

#ifdef _WIN32
/* -------------------------------------------------------------
 * Win32 SEH ({see: os/Windows.h})
 * -------------------------------------------------------------
 * Add support for Windows SEH through an interface similar to
 * signal.h
 */
#include "Windows.h"
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
#endif

#endif /* FL_SIGNAL_H */