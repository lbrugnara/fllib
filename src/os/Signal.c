#include "Signal.h"

void fl_signal_handler_set(int sig, FlSignalHandler handler)
{
    signal(sig, (__p_sig_fn_t)handler);
}

void fl_signal_global_handler_set(FlSignalHandler handler)
{
    signal(SIGABRT, (__p_sig_fn_t)handler);
    signal(SIGFPE, (__p_sig_fn_t)handler);
    signal(SIGILL, (__p_sig_fn_t)handler);
    signal(SIGINT, (__p_sig_fn_t)handler);
    signal(SIGSEGV, (__p_sig_fn_t)handler);
    signal(SIGTERM, (__p_sig_fn_t)handler);
}