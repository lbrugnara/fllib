#include "Signal.h"

void fl_signal_set_handler(unsigned long sig, FlSignalHandler handler)
{
    #ifdef _WIN32
        fl_exception_handler_set(sig, handler);
    #else
        signal(sig, (__p_sig_fn_t)handler);
    #endif
}