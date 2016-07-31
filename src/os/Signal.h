#ifndef FL_SIGNAL_H
#define FL_SGINAL_H

#ifdef _WIN32
    #include "Windows.h"
    /* -------------------------------------------------------------
     * Win32 SEH ({see: os/Windows.h})
     * -------------------------------------------------------------
     */
#else
    #include <signal.h>
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
    typedef void(*FlSignalHandler)(int sig);
#endif

void fl_signal_set_handler(unsigned long sig, FlSignalHandler handler);

#endif /* FL_SIGNAL_H */