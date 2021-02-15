#include <stdio.h>

#include "../src/Std.h"
#include "../src/Cstring.h"
#include "../src/Array.h"
#include "../src/threading/Thread.h"

#include <flut/flut.h>

flut_suite(std) {
    flut_suite_description("fllib standard functions");
    flut_suite_register_test(std_exceptions, "Global error handling thread safety");
    flut_suite_register_test(std_thread_errors, "Pseudo try-catch exception handling");
    flut_suite_register_test(std_scoped_resources, "Scoped resources");
}

// std_scoped_resources
#define FL_STD_TEST_MAX_ALLOCATIONS 10
#define FL_STD_TEST_ALLOC_HEADER(a) (((struct StdTestAllocation*)a)-1)
#define FL_STD_TEST_ALLOC_DATA(h) (((struct StdTestAllocation*)h) + 1)

static void **allocations[FL_STD_TEST_MAX_ALLOCATIONS];

struct StdTestAllocation {
    bool is_allocated;
};

static void* allocate(size_t size);
static bool is_allocated(size_t index);
static void deallocate(void *data);
static void init();
static void end();

// std_thread_errors
void thread_error(void *args);

// std_exceptions
void a(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM);
void b(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM);
void c(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM);
void d(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM);
void e(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM);


flut_test(std_thread_errors) {
    int nthreads = 5;
    FlThread *threads = fl_array_new(sizeof(FlThread), nthreads);
    for (int i=0; i < nthreads; i++)
    {
        threads[i] = fl_thread_create(thread_error, FLUT_TEST_CONTEXT);
    }
    fl_thread_join_all(threads, nthreads);
    
    for (int i=0; i < nthreads; i++)
        fl_thread_free(threads[i]);

    fl_array_free(threads);
}

flut_test(std_exceptions) {
    flut_describe("Passing the context all along function calls should result in an ordered \"unwind\" of the exception") {
        struct FlContext *try_ctx = fl_ctx_new();

        Try(try_ctx)
        {
            a(try_ctx, FLUT_TEST_CONTEXT);
        }
        Catch((int)'a')
        {
            struct FlContextFrame *frame = fl_ctx_frame_last(try_ctx);
            flut_assert_is_true(frame->exception == (int)'a');
            flut_assert_is_true(flm_cstring_equals_n(frame->message, "a", 1));
        }
        Rest
        {
            flut_unexpected("The exception shouldn't have fallen in root's Rest clause");
        }
        EndTry;

        fl_ctx_free(try_ctx);
    }

    flut_describe("Nested Try calls should result in an ordered \"unwind\" of the exception") {
        struct FlContext *try_ctx = fl_ctx_new();

        Try(try_ctx)
        {
            Try(try_ctx) { 
                Try(try_ctx) { 
                    Try(try_ctx) { 

                    }
                    EndTry; 
                } 
                EndTry; 
            } 
            EndTry;
        }
        EndTry;

        fl_ctx_free(try_ctx);
    }
}

flut_test(std_scoped_resources) {
    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope under normal flow execution") {
        
        flut_assert_is_false(is_allocated(0));

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_is_true(is_allocated(0));
         flut_assert_is_false(is_allocated(1));
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_is_true(is_allocated(1));
          flut_assert_is_false(is_allocated(2));
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_is_true(is_allocated(2));
           flut_assert_is_false(is_allocated(3));
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_is_true(is_allocated(3));
            flut_assert_is_false(is_allocated(4));
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_is_true(is_allocated(4));
             flut_assert_is_false(is_allocated(5));
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_is_true(is_allocated(5));
              flut_assert_is_false(is_allocated(6));
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_is_true(is_allocated(6));
               flut_assert_is_false(is_allocated(7));
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_is_true(is_allocated(7));
                flut_assert_is_false(is_allocated(8));
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_is_true(is_allocated(8));
                 flut_assert_is_false(is_allocated(9));
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_is_null(allocate(sizeof(void*)));
                   // End res10
                 }

                 flut_assert_is_false(is_allocated(9));
                 flut_assert_is_true(is_allocated(8));
                 // End res9
                }

                flut_assert_is_false(is_allocated(8));
                flut_assert_is_true(is_allocated(7));
                // End res8
               }

               flut_assert_is_false(is_allocated(7));
               flut_assert_is_true(is_allocated(6));
               // End res7
              }

              flut_assert_is_false(is_allocated(6));
              flut_assert_is_true(is_allocated(5));
              // End res6
             }

             flut_assert_is_false(is_allocated(5));
             flut_assert_is_true(is_allocated(4));
             // End res5
            }

            flut_assert_is_false(is_allocated(4));
            flut_assert_is_true(is_allocated(3));
            // End res4
           }

           flut_assert_is_false(is_allocated(3));
           flut_assert_is_true(is_allocated(2));
           // End res3
          }

          flut_assert_is_false(is_allocated(2));
          flut_assert_is_true(is_allocated(1));
          // End res2
         }

         flut_assert_is_false(is_allocated(1));
         flut_assert_is_true(is_allocated(0));
         // End res1
        }

        flut_assert_is_false(is_allocated(0));
    }
    end();

    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope using break (WEIRD BUT EXPECTED)") {
        
        flut_assert_is_false(is_allocated(0));

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_is_true(is_allocated(0));
         flut_assert_is_false(is_allocated(1));
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_is_true(is_allocated(1));
          flut_assert_is_false(is_allocated(2));
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_is_true(is_allocated(2));
           flut_assert_is_false(is_allocated(3));
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_is_true(is_allocated(3));
            flut_assert_is_false(is_allocated(4));
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_is_true(is_allocated(4));
             flut_assert_is_false(is_allocated(5));
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_is_true(is_allocated(5));
              flut_assert_is_false(is_allocated(6));
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_is_true(is_allocated(6));
               flut_assert_is_false(is_allocated(7));
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_is_true(is_allocated(7));
                flut_assert_is_false(is_allocated(8));
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_is_true(is_allocated(8));
                 flut_assert_is_false(is_allocated(9));
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_is_null(allocate(sizeof(void*)));
                   break;
                   // End res10
                 }

                 flut_assert_is_false(is_allocated(9));
                 flut_assert_is_true(is_allocated(8));
                 break;
                 // End res9
                }

                flut_assert_is_false(is_allocated(8));
                flut_assert_is_true(is_allocated(7));
                break;
                // End res8
               }

               flut_assert_is_false(is_allocated(7));
               flut_assert_is_true(is_allocated(6));
               break;
               // End res7
              }

              flut_assert_is_false(is_allocated(6));
              flut_assert_is_true(is_allocated(5));
              break;
              // End res6
             }

             flut_assert_is_false(is_allocated(5));
             flut_assert_is_true(is_allocated(4));
             break;
             // End res5
            }

            flut_assert_is_false(is_allocated(4));
            flut_assert_is_true(is_allocated(3));
            break;
            // End res4
           }

           flut_assert_is_false(is_allocated(3));
           flut_assert_is_true(is_allocated(2));
           break;
           // End res3
          }

          flut_assert_is_false(is_allocated(2));
          flut_assert_is_true(is_allocated(1));
          break;
          // End res2
         }

         flut_assert_is_false(is_allocated(1));
         flut_assert_is_true(is_allocated(0));
         break;
         // End res1
        }

        flut_assert_is_false(is_allocated(0));
        break;
    }
    end();

    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope using continue (UNEXPECTED)") {
        
        flut_assert_is_false(is_allocated(0));

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_is_true(is_allocated(0));
         flut_assert_is_false(is_allocated(1));
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_is_true(is_allocated(1));
          flut_assert_is_false(is_allocated(2));
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_is_true(is_allocated(2));
           flut_assert_is_false(is_allocated(3));
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_is_true(is_allocated(3));
            flut_assert_is_false(is_allocated(4));
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_is_true(is_allocated(4));
             flut_assert_is_false(is_allocated(5));
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_is_true(is_allocated(5));
              flut_assert_is_false(is_allocated(6));
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_is_true(is_allocated(6));
               flut_assert_is_false(is_allocated(7));
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_is_true(is_allocated(7));
                flut_assert_is_false(is_allocated(8));
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_is_true(is_allocated(8));
                 flut_assert_is_false(is_allocated(9));
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_is_null(allocate(sizeof(void*)));
                   continue;
                   // End res10
                 }

                 flut_assert_is_false(is_allocated(9));
                 flut_assert_is_true(is_allocated(8));
                 continue;
                 // End res9
                }

                flut_assert_is_false(is_allocated(8));
                flut_assert_is_true(is_allocated(7));
                continue;
                // End res8
               }

               flut_assert_is_false(is_allocated(7));
               flut_assert_is_true(is_allocated(6));
               continue;
               // End res7
              }

              flut_assert_is_false(is_allocated(6));
              flut_assert_is_true(is_allocated(5));
              continue;
              // End res6
             }

             flut_assert_is_false(is_allocated(5));
             flut_assert_is_true(is_allocated(4));
             continue;
             // End res5
            }

            flut_assert_is_false(is_allocated(4));
            flut_assert_is_true(is_allocated(3));
            continue;
            // End res4
           }

           flut_assert_is_false(is_allocated(3));
           flut_assert_is_true(is_allocated(2));
           continue;
           // End res3
          }

          flut_assert_is_false(is_allocated(2));
          flut_assert_is_true(is_allocated(1));
          continue;
          // End res2
         }

         flut_assert_is_false(is_allocated(1));
         flut_assert_is_true(is_allocated(0));
         continue;
         // End res1
        }

        flut_assert_is_false(is_allocated(0));
        continue;
    }
    end();
}


void thread_error(void *args)
{
    FLUT_TEST_CONTEXT_PARAM = (FlutContext*) args;
    struct FlError error;
    char *str;
    FlThreadId id = fl_thread_current_id();
    fl_error_push(1, "ID=%d | Testing error 1", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 1", id);
    flut_assert_int_is_equals(1, (int) error.id);
    flut_assert_string_is_equals(str, error.message, false);
    fl_cstring_free(str);
    
    fl_error_push(2, "ID=%d | Testing error 2", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 2", id);
    flut_assert_int_is_equals(2, (int) error.id);
    flut_assert_string_is_equals(str, error.message, false);
    fl_cstring_free(str);
    
    fl_error_push(3, "ID=%d | Testing error 3", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 3", id);
    flut_assert_int_is_equals(3, (int) error.id);
    flut_assert_string_is_equals(str, error.message, false);
    fl_cstring_free(str);
    
    fl_error_push(4, "ID=%d | Testing error 4", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 4", id);
    flut_assert_int_is_equals(4, (int) error.id);
    flut_assert_string_is_equals(str, error.message, false);
    fl_cstring_free(str);
    
    fl_error_push(5, "ID=%d | Testing error 5", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 5", id);
    flut_assert_int_is_equals(5, (int) error.id);
    flut_assert_string_is_equals(str, error.message, false);
    fl_cstring_free(str);

    fl_thread_exit(NULL);
}

void a(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM) {
    Try(ctx)
    {
        b(ctx, FLUT_TEST_CONTEXT);
    }
    Catch((int)'b')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_assert_is_true(frame->exception == (int)'b');
        flut_assert_is_true(flm_cstring_equals_n(frame->message, "b", 1));
        Throw(ctx, (int)'a', "a");
    }
    Rest
    {
        flut_unexpected("The exception shouldn't have fallen in a()'s Rest clause");
    }
    EndTry;
}

void b(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM) {
    Try(ctx)
    {
        c(ctx, FLUT_TEST_CONTEXT);
    }
    Catch((int)'c')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_assert_is_true(frame->exception == (int)'c');
        flut_assert_is_true(flm_cstring_equals_n(frame->message, "c", 1));
        Throw(ctx, (int)'b', "b");
    }
    Rest
    {
        flut_unexpected("The exception shouldn't have fallen in b()'s Rest clause");
    }
    EndTry;
}

void c(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM) { 
    Try(ctx)
    {   
        d(ctx, FLUT_TEST_CONTEXT);
    }
    Catch((int)'d')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_assert_is_true(frame->exception == (int)'d');
        flut_assert_is_true(flm_cstring_equals_n(frame->message, "d", 1));
        Throw(ctx, (int)'c', "c");
    }
    Rest
    {
        flut_unexpected("The exception shouldn't have fallen in c()'s Rest clause");
    }
    EndTry;
}

void d(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM) {
    Try(ctx)
    {
        e(ctx, FLUT_TEST_CONTEXT);
    }
    Catch((int)'e')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_assert_is_true(frame->exception == (int)'e');
        flut_assert_is_true(flm_cstring_equals_n(frame->message, "e", 1));
        Throw(ctx, (int)'d', "d");
    }
    Rest
    {
        flut_unexpected("The exception shouldn't have fallen in d()'s Rest clause");
    }
    EndTry;
}

void e(struct FlContext *ctx, FLUT_TEST_CONTEXT_PARAM) {
    Throw(ctx, (int)'e', "e");
}

static void* allocate(size_t size)
{
    size_t index = 0;
    for (; index < FL_STD_TEST_MAX_ALLOCATIONS; index++) {
        if (allocations[index] == NULL) {
            break;
        }
    }

    if (index == FL_STD_TEST_MAX_ALLOCATIONS)
        return NULL;

    struct StdTestAllocation *allocation_header = fl_calloc(1, sizeof(struct StdTestAllocation) + size);
    allocation_header->is_allocated = true;    

    allocations[index] = (void*) FL_STD_TEST_ALLOC_DATA(allocation_header);

    return allocations[index];
}

static bool is_allocated(size_t index)
{
    return index < FL_STD_TEST_MAX_ALLOCATIONS && allocations[index] != NULL && FL_STD_TEST_ALLOC_HEADER(allocations[index])->is_allocated;
}

static void deallocate(void *data)
{
    struct StdTestAllocation *allocation_header = FL_STD_TEST_ALLOC_HEADER(data);
    allocation_header->is_allocated = false;
}

static void init()
{
    memset(allocations, 0, sizeof(void*) * 10);
}

static void end()
{
    for (size_t i = 0; i < FL_STD_TEST_MAX_ALLOCATIONS; i++)
    {
        if (allocations[i] == NULL)
            continue;

        fl_free(FL_STD_TEST_ALLOC_HEADER(allocations[i]));
    }
}
