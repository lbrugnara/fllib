#include <stdlib.h>
#include "Timer.h"
#include "../Error.h"
#include "../Mem.h"
#include "../Std.h"

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <sys/time.h>
    #ifndef __USE_POSIX199309
    #define __USE_POSIX199309
    #endif
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

FlTimer* fl_timer_create()
{
    FlTimer *timer = fl_malloc(sizeof(struct FlTimer));
    timer->status = TIMER_STOPPED;
    return timer;
}

void fl_timer_start(FlTimer *timer)
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

void fl_timer_end(FlTimer *timer)
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

unsigned long fl_timer_tick_ms(FlTimer *timer)
{
    flm_assert(timer != NULL, "Timer must not be null");

    // Tick (re)starts the timer
    if (timer->status == TIMER_STOPPED || timer->status == TIMER_ENDED)
    {
        fl_timer_start(timer);
        return 0;
    }

    fl_timer_end(timer);
    unsigned long elapsed = fl_timer_elapsed_ms(timer);
    fl_timer_start(timer);

    return elapsed;
}

unsigned long long fl_timer_tick_ns(FlTimer *timer)
{
    flm_assert(timer != NULL, "Timer must not be null");

    // Tick (re)starts the timer
    if (timer->status == TIMER_STOPPED || timer->status == TIMER_ENDED)
    {
        fl_timer_start(timer);
        return 0;
    }

    fl_timer_end(timer);
    unsigned long long elapsed = fl_timer_elapsed_ns(timer);
    fl_timer_start(timer);

    return elapsed;
}

unsigned long fl_timer_elapsed_ms(FlTimer *timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    flm_assert(timer->status == TIMER_ENDED, "Timer stopped or already running");
    #ifdef _WIN32
        ULONG elapsed= timer->end.QuadPart - timer->start.QuadPart;
        elapsed *= 1000;
        elapsed /= timer->freq.QuadPart;
        return elapsed;
    #elif defined(__linux__) || defined(__CYGWIN__)
        unsigned long start_ms = timer->start.tv_nsec / 1.0e6;
        start_ms += (timer->start.tv_sec * 1000);
        unsigned long end_ms = timer->end.tv_nsec / 1.0e6;
        end_ms += (timer->end.tv_sec * 1000);
        return end_ms - start_ms;
    #endif
}

unsigned long long fl_timer_elapsed_ns(FlTimer *timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    flm_assert(timer->status == TIMER_ENDED, "Timer stopped or already running");
    #ifdef _WIN32
        ULONGLONG elapsed = timer->end.QuadPart - timer->start.QuadPart;
        elapsed *= 1000000000;
        elapsed /= timer->freq.QuadPart;
        return elapsed;
    #elif defined(__linux__) || defined(__CYGWIN__)
        unsigned long long start_ns = timer->start.tv_nsec / 1.0e6;
        start_ns += (timer->start.tv_sec * 1000000000);
        unsigned long long end_ns = timer->end.tv_nsec / 1.0e6;
        end_ns += (timer->end.tv_sec * 1000000000);
        return end_ns - start_ns;
    #endif
}

void fl_timer_free(FlTimer *timer)
{
    flm_assert(timer != NULL, "Timer must not be null");
    fl_free(timer);
}
