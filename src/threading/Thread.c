#include <stdio.h>
#include <stdbool.h>
#include "../Types.h"
#include "../Mem.h"
#include "Thread.h"

FlThread fl_thread_create(FlThreadFunc routine, FlThreadArgs args)
{
    FlThread thread;
    #ifdef FL_WIN_THREADS
        thread = (FlThread)((uintptr_t)_beginthreadex(NULL, 0, (unsigned(__stdcall*)(void*))routine, args, 0, NULL));
    #elif defined(FL_PTHREADS)
        pthread_create(&thread, NULL, (void*(*)(void*))routine, args);
    #elif defined(FL_NO_THREADS)
        routine(args);
        thread = 1;
    #endif
    return thread;
}

void fl_thread_free(FlThread thread)
{
    #ifdef _WIN32
        if (!CloseHandle((HANDLE)(uintptr_t)thread))
            fprintf(stderr, "Cannot destroy handle");
    #endif
}

FlThreadId fl_thread_current_id()
{
    FlThreadId threadId;
    #ifdef _WIN32
        // If platform is _WIN32 and compilation flag is (or not) FL_PTHREADS
        // use GetCurrentThreadId(), because gettid is not available
        threadId = GetCurrentThreadId();
    #elif defined(FL_PTHREADS) && !defined(_WIN32)
        threadId = pthread_self();
    #elif defined(FL_NO_THREADS)
        threadId = 1;
    #endif
    return threadId;
}

void fl_thread_exit(void *retval)
{
    #ifdef FL_WIN_THREADS
        _endthreadex(retval ? *(unsigned int*)retval : 0);
    #elif defined(FL_PTHREADS)
        pthread_exit(retval);
    #endif
}

void fl_thread_join(FlThread thread)
{
    #ifdef FL_WIN_THREADS
        WaitForSingleObject(thread, INFINITE);
    #elif defined(FL_PTHREADS)
        pthread_join(thread, NULL);
    #endif
}

// TODO: Add WaitAll boolean, and return type different from void
// Keep in mind pthread_join is not similar to WaitForMultipleObjects
// it should be implemented with a condition
bool fl_thread_join_all(FlThread *threads, size_t nthreads)
{
    #ifdef FL_WIN_THREADS
        // WaitForMultipleObjects waits up to 64 objects
        //DWORD result = WaitForMultipleObjects((DWORD)nthreads, (HANDLE*)threads, TRUE, INFINITE);
        //return result != WAIT_TIMEOUT && result != WAIT_FAILED && result != WAIT_ABANDONED_0;
        for (size_t i=0; i < nthreads; i++)
        {
            WaitForSingleObject(threads[i], INFINITE);            
        }
    #elif defined(FL_PTHREADS)
        for (size_t i=0; i < nthreads; i++)
        {
            pthread_join(threads[i], NULL);
        }
    #endif
    // TODO: Return value 
    return true; 
}

void fl_mutex_init(FlMutex *mutex)
{
    #ifdef FL_WIN_MUTEX
        *mutex = CreateMutex(NULL, FALSE, NULL);
    #elif defined(FL_WIN_THREADS)
        *mutex = fl_malloc(sizeof(CRITICAL_SECTION));
        InitializeCriticalSection(*mutex);
    #elif defined(FL_PTHREADS)
        pthread_mutex_init(mutex, NULL);
    #endif
}

void fl_mutex_lock(FlMutex *mutex)
{
    #ifdef FL_WIN_MUTEX
        if (*mutex == NULL)
        {
            HANDLE p = CreateMutex(NULL, FALSE, NULL);
            if (InterlockedCompareExchangePointer((PVOID*)mutex, (PVOID)p, NULL) != NULL)
                CloseHandle(p);
        }
        WaitForSingleObject(*mutex, INFINITE);
    #elif defined(FL_WIN_THREADS)
        if (*mutex == NULL)
        { 
            CRITICAL_SECTION *s = fl_malloc(sizeof(CRITICAL_SECTION));
            InitializeCriticalSection(s);
            if (InterlockedCompareExchangePointer((PVOID*)mutex, (PVOID)s, NULL) != NULL)
            {
                DeleteCriticalSection(s);
                free(s);
            }
        }
        EnterCriticalSection(*mutex);
    #elif defined(FL_PTHREADS)
        pthread_mutex_lock(mutex);
    #endif   
}

void fl_mutex_unlock(FlMutex *mutex)
{
    #ifdef FL_WIN_MUTEX
        ReleaseMutex(*mutex);
    #elif defined(FL_WIN_THREADS)
        LeaveCriticalSection(*mutex);
    #elif defined(FL_PTHREADS)
        pthread_mutex_unlock(mutex);
    #endif   
}

void fl_mutex_destroy(FlMutex *mutex)
{
    #ifdef FL_WIN_MUTEX
        CloseHandle(*mutex);
    #elif defined(FL_WIN_THREADS)
        DeleteCriticalSection(*mutex);
        fl_free(*mutex);
    #elif defined(FL_PTHREADS)
        pthread_mutex_destroy(mutex);
    #endif
}
