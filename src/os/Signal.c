#include "Signal.h"

void fl_signal_handler_set(int sig, FlSignalHandler handler)
{
    signal(sig, handler);
}

void fl_signal_global_handler_set(FlSignalHandler handler)
{
    signal(SIGABRT, handler);
    signal(SIGFPE, handler);
    signal(SIGILL, handler);
    signal(SIGINT, handler);
    signal(SIGSEGV, handler);
    signal(SIGTERM, handler);
}