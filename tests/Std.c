#include <stdio.h>

#include "../src/Std.h"
#include "../src/Cstring.h"
#include "../src/Array.h"
#include "../src/threading/Thread.h"
#include "Std.h"

#include "Test.h"

void thread_error(void *args)
{
    struct FlError error;
    char *str;
    FlThreadId id = fl_thread_current_id();
    fl_error_push(1, "ID=%d | Testing error 1", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 1", id);
    fl_expect(str, error.id == 1 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(2, "ID=%d | Testing error 2", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 2", id);
    fl_expect(str, error.id == 2 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(3, "ID=%d | Testing error 3", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 3", id);
    fl_expect(str, error.id == 3 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(4, "ID=%d | Testing error 4", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 4", id);
    fl_expect(str, error.id == 4 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(5, "ID=%d | Testing error 5", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 5", id);
    fl_expect(str, error.id == 5 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);

    fl_thread_exit(NULL);
}

void test_errors()
{
    int nthreads = 5;
    FlThread *threads = fl_array_new(sizeof(FlThread), nthreads);
    for (int i=0; i < nthreads; i++)
    {
        threads[i] = fl_thread_create(thread_error, NULL);
    }
    fl_thread_join_all(threads, nthreads);
    
    for (int i=0; i < nthreads; i++)
        fl_thread_delete(threads[i]);

    fl_array_free(threads);
}


void a(struct FlContext *ctx);
void b(struct FlContext *ctx);
void c(struct FlContext *ctx);
void d(struct FlContext *ctx);
void e(struct FlContext *ctx);

void test_std_exception()
{
    struct FlContext *ctx = fl_ctx_new();

    Try(ctx)
    {
        a(ctx);
    }
    Catch((int)'a')
    {
        struct FlContextFrame *frame = fl_ctx_last_frame(ctx);
        fl_expect("Catched exception in root should be (int)'a'", frame->exception == (int)'a');
        fl_expect("Catched exception in root should have message \"a\"", flm_cstring_equals_n(frame->message, "a", 1));
    }
    Rest
    {
        fl_expect("Exception shouldn't have fallen in root's Rest clause", false);
    }
    EndTry;

    fl_ctx_free(ctx);
}

void a(struct FlContext *ctx) 
{
    Try(ctx)
    {
        b(ctx);
    }
    Catch((int)'b')
    {
        struct FlContextFrame *frame = fl_ctx_last_frame(ctx);
        fl_expect("Catched exception in a() should be (int)'b'", frame->exception == (int)'b');
        fl_expect("Catched exception in a() should have message \"b\"", flm_cstring_equals_n(frame->message, "b", 1));
        Throw(ctx, (int)'a', "a");
    }
    Rest
    {
        fl_expect("Exception shouldn't have fallen in a()'s Rest clause", false);
    }
    EndTry;
}

void b(struct FlContext *ctx) 
{
    Try(ctx)
    {
        c(ctx);
    }
    Catch((int)'c')
    {
        struct FlContextFrame *frame = fl_ctx_last_frame(ctx);
        fl_expect("Catched exception in b() should be (int)'c'", frame->exception == (int)'c');
        fl_expect("Catched exception in b() should have message \"c\"", flm_cstring_equals_n(frame->message, "c", 1));
        Throw(ctx, (int)'b', "b");
    }
    Rest
    {
        fl_expect("Exception shouldn't have fallen in b()'s Rest clause", false);
    }
    EndTry;
}

void c(struct FlContext *ctx) 
{ 
    Try(ctx)
    {   
        d(ctx);
    }
    Catch((int)'d')
    {
        struct FlContextFrame *frame = fl_ctx_last_frame(ctx);
        fl_expect("Catched exception in c() should be (int)'d'", frame->exception == (int)'d');
        fl_expect("Catched exception in c() should have message \"d\"", flm_cstring_equals_n(frame->message, "d", 1));
        Throw(ctx, (int)'c', "c");
    }
    Rest
    {
        fl_expect("Exception shouldn't have fallen in c()'s Rest clause", false);
    }
    EndTry;
}

void d(struct FlContext *ctx) 
{
    Try(ctx)
    {
        e(ctx);
    }
    Catch((int)'e')
    {
        struct FlContextFrame *frame = fl_ctx_last_frame(ctx);
        fl_expect("Catched exception in d() should be (int)'e'", frame->exception == (int)'e');
        fl_expect("Catched exception in d() should have message \"e\"", flm_cstring_equals_n(frame->message, "e", 1));
        Throw(ctx, (int)'d', "d");
    }
    Rest
    {
        fl_expect("Exception shouldn't have fallen in d()'s Rest clause", false);
    }
    EndTry;
}

void e(struct FlContext *ctx) 
{
    Throw(ctx, (int)'e', "e");
}
