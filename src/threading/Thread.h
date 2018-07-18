#ifndef FL_THREADS_H
#define FL_THREADS_H

// MinGW supports a pthreads implementation
// if FL_PTHREADS is defined we will use it, if not
// we will use Windows threads
#if _WIN32 && !defined(FL_PTHREADS)
    #define FL_WIN_THREADS
#elif defined(__linux__) || defined(__CYGWIN__)
    #define FL_PTHREADS
#else
    #define FL_NO_THREADS
#endif

#ifdef FL_WIN_THREADS
    #include <windows.h>
    #include <process.h>
    #include <stdint.h>

    typedef HANDLE FlThread;
    typedef DWORD FlThreadId;
    #ifdef FL_WIN_MUTEX
        typedef HANDLE FlMutex;
        #define FL_MUTEX_INIT_STATIC NULL
    #else
        // Add an indirection to allow "copying" ciritcal sections.
        // This way, FlMutex is not limited by windows implementation 
        // in other platforms. This allows code like:
        //
        //      void routine(FlThreadArgs args)
        //      {
        //          FlMutex mutex = *(FlMutex*)args
        //          fl_mutex_lock(&mutex);
        //          // critical section ...
        //          fl_mutex_unlock(&mutex);
        //          // Cleanup ...
        //      }
        //
        //      void main()
        //      {
        //          FlMutex mutex;
        //          // init mutex
        //          fl_thread_create(routine, &mutex);
        //          // Cleanup ...
        //      }
        //
        typedef CRITICAL_SECTION* FlMutex;
        #define FL_MUTEX_INIT_STATIC NULL
    #endif

#elif defined(FL_PTHREADS)
    #ifdef _WIN32
        #include <windows.h>
        #define HAVE_STRUCT_TIMESPEC // pthreads-w32 tries to redeclare struct timespec
    #else
        #include <unistd.h>
        #include <sys/types.h>
    #endif
    #include <pthread.h>
    
    typedef pthread_t FlThread;
    typedef pthread_t FlThreadId;
    typedef pthread_mutex_t FlMutex;
    #define FL_MUTEX_INIT_STATIC PTHREAD_MUTEX_INITIALIZER
#elif defined(FL_NO_THREADS)
    #include <stddef.h>

    typedef int FlThread;
    typedef int FlThreadId;
    typedef int FlMutex;
    #define FL_MUTEX_INIT_STATIC 0
#endif

typedef void(*FlThreadFunc)(void*);
typedef void* FlThreadArgs;

FlThread fl_thread_create(FlThreadFunc routine, FlThreadArgs args);
void fl_thread_delete(FlThread thread);
FlThreadId fl_thread_current_id();
void fl_thread_exit(void *retval);
void fl_thread_join(FlThread thread);
bool fl_thread_join_all(FlThread *threads, size_t nthreads);

void fl_mutex_init(FlMutex *mutex);
void fl_mutex_lock(FlMutex *mutex);
void fl_mutex_unlock(FlMutex *mutex);
void fl_mutex_destroy(FlMutex *mutex);

#endif /* FL_THREADS_H */
