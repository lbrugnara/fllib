#ifndef FL_SIGNAL_H
#define FL_SIGNAL_H

/*
 * C99 Signals
 *
 * SIGABRT
 * SIGFPE
 * SIGILL
 * SIGINT
 * SIGSEGV
 * SIGTERM
 */
#include <signal.h>

/*
 * Type: void (*)(int) FlSignalHandler
 *
 * Signals handler function. Used to call signal(3)
 *
 */
typedef void(*FlSignalHandler)(int sig);

/*
 * Function: fl_signal_handler_set
 *
 * Set the FlSignalHandler for a specific signal
 *
 * int sig - Signal number
 * FlSignalHandler handler - Signal handler function
 *
 * {return: void}
 *
 */
void fl_signal_handler_set(int sig, FlSignalHandler handler);

/*
 * Function: fl_signal_global_handler_set
 *
 * Set an FlSignalHandler for all the signals defined in
 * the C standard
 *
 * FlSignalHandler handler - Signal handler function
 *
 * {return: void}
 *
 */
void fl_signal_global_handler_set(FlSignalHandler handler);

#endif /* FL_SIGNAL_H */
