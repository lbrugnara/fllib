#include <stdlib.h>
#include "Timer.h"
#include "../Std.h"
#include "../Mem.h"

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <time.h>
#endif

enum TimerStatus {
    TIMER_STOPPED,
    TIMER_STARTED,
    TIMER_ENDED
};

struct FlTimer {
    #ifdef _WIN32
        LARGE_INTEGER start;
        LARGE_INTEGER end;
        LARGE_INTEGER freq;
    #elif defined(__linux__) || defined(__CYGWIN__)
        struct timespec start;
        struct timespec end;
    #endif
    enum TimerStatus status;
};

FlTimer fl_timer_create()
{
    FlTimer timer = fl_malloc(sizeof(struct FlTimer));
    timer->status = TIMER_STOPPED;
    return timer;
}

void fl_timer_start(FlTimer timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    timer->status = TIMER_STARTED;
    #ifdef _WIN32
        QueryPerformanceFrequency(&(timer->freq));
        QueryPerformanceCounter(&(timer->start));
    #elif defined(__linux__) || defined(__CYGWIN__)
        clock_gettime(CLOCK_MONOTONIC, &(timer->start));
    #endif
}

void fl_timer_end(FlTimer timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    flm_assert(timer->status == TIMER_STARTED, "Timer needs to be running");
    #ifdef _WIN32
        QueryPerformanceCounter(&(timer->end));
    #elif defined(__linux__) || defined(__CYGWIN__)
        clock_gettime(CLOCK_MONOTONIC, &(timer->end));
    #endif
    timer->status = TIMER_ENDED;
}

long fl_timer_elapsed_ms(FlTimer timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    flm_assert(timer->status == TIMER_ENDED, "Timer stopped or already running");
    #ifdef _WIN32
        LONGLONG elapsed= timer->end.QuadPart - timer->start.QuadPart;
        elapsed *= 1000;
        elapsed /= timer->freq.QuadPart;
        return elapsed;
    #elif defined(__linux__) || defined(__CYGWIN__)
        long startms = timer->start.tv_nsec / 1.0e6;
        startms += (timer->start.tv_sec * 1000);
        long endms = timer->end.tv_nsec / 1.0e6;
        endms += (timer->end.tv_sec * 1000);
        return endms - startms;
    #endif
}

void fl_timer_free(FlTimer timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    fl_free(timer);
}
