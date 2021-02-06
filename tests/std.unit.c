#include <stdio.h>

#include "../src/Std.h"
#include "../src/Cstring.h"
#include "../src/Array.h"
#include "../src/threading/Thread.h"

#include <flut/flut.h>

flut_define_suite(std) {
    flut_suite_description("fllib standard functions");
    flut_suite_register_test(std_exceptions, "Global error handling thread safety");
    flut_suite_register_test(std_thread_errors, "Pseudo try-catch exception handling");
    flut_suite_register_test(std_scoped_resources, "Scoped resources");
}

void thread_error(void *args)
{
    struct FlError error;
    char *str;
    FlThreadId id = fl_thread_current_id();
    fl_error_push(1, "ID=%d | Testing error 1", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 1", id);
    flut_expect_compat(str, error.id == 1 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(2, "ID=%d | Testing error 2", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 2", id);
    flut_expect_compat(str, error.id == 2 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(3, "ID=%d | Testing error 3", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 3", id);
    flut_expect_compat(str, error.id == 3 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(4, "ID=%d | Testing error 4", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 4", id);
    flut_expect_compat(str, error.id == 4 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);
    
    fl_error_push(5, "ID=%d | Testing error 5", id);
    error = fl_error_last();
    str = fl_cstring_vdup("ID=%d | Testing error 5", id);
    flut_expect_compat(str, error.id == 5 && flm_cstring_equals(error.message, str));
    fl_cstring_free(str);

    fl_thread_exit(NULL);
}

flut_define_test(std_thread_errors) {
    int nthreads = 5;
    FlThread *threads = fl_array_new(sizeof(FlThread), nthreads);
    for (int i=0; i < nthreads; i++)
    {
        threads[i] = fl_thread_create(thread_error, NULL);
    }
    fl_thread_join_all(threads, nthreads);
    
    for (int i=0; i < nthreads; i++)
        fl_thread_free(threads[i]);

    fl_array_free(threads);
}


void a(struct FlContext *ctx);
void b(struct FlContext *ctx);
void c(struct FlContext *ctx);
void d(struct FlContext *ctx);
void e(struct FlContext *ctx);

flut_define_test(std_exceptions) {
    struct FlContext *try_ctx = fl_ctx_new();

    Try(try_ctx)
    {
        a(try_ctx);
    }
    Catch((int)'a')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(try_ctx);
        flut_expect_compat("Catched exception in root should be (int)'a'", frame->exception == (int)'a');
        flut_expect_compat("Catched exception in root should have message \"a\"", flm_cstring_equals_n(frame->message, "a", 1));
    }
    Rest
    {
        flut_expect_compat("The exception shouldn't have fallen in root's Rest clause", false);
    }
    EndTry;

    fl_ctx_free(try_ctx);

    try_ctx = fl_ctx_new();

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

void a(struct FlContext *ctx) 
{
    Try(ctx)
    {
        b(ctx);
    }
    Catch((int)'b')
    {
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_expect_compat("Catched exception in a() should be (int)'b'", frame->exception == (int)'b');
        flut_expect_compat("Catched exception in a() should have message \"b\"", flm_cstring_equals_n(frame->message, "b", 1));
        Throw(ctx, (int)'a', "a");
    }
    Rest
    {
        flut_expect_compat("The exception shouldn't have fallen in a()'s Rest clause", false);
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
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_expect_compat("Catched exception in b() should be (int)'c'", frame->exception == (int)'c');
        flut_expect_compat("Catched exception in b() should have message \"c\"", flm_cstring_equals_n(frame->message, "c", 1));
        Throw(ctx, (int)'b', "b");
    }
    Rest
    {
        flut_expect_compat("The exception shouldn't have fallen in b()'s Rest clause", false);
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
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_expect_compat("Catched exception in c() should be (int)'d'", frame->exception == (int)'d');
        flut_expect_compat("Catched exception in c() should have message \"d\"", flm_cstring_equals_n(frame->message, "d", 1));
        Throw(ctx, (int)'c', "c");
    }
    Rest
    {
        flut_expect_compat("The exception shouldn't have fallen in c()'s Rest clause", false);
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
        struct FlContextFrame *frame = fl_ctx_frame_last(ctx);
        flut_expect_compat("Catched exception in d() should be (int)'e'", frame->exception == (int)'e');
        flut_expect_compat("Catched exception in d() should have message \"e\"", flm_cstring_equals_n(frame->message, "e", 1));
        Throw(ctx, (int)'d', "d");
    }
    Rest
    {
        flut_expect_compat("The exception shouldn't have fallen in d()'s Rest clause", false);
    }
    EndTry;
}

void e(struct FlContext *ctx) 
{
    Throw(ctx, (int)'e', "e");
}

#define FL_STD_TEST_MAX_ALLOCATIONS 10

static void **allocations[FL_STD_TEST_MAX_ALLOCATIONS];

struct StdTestAllocation {
    bool is_allocated;
};

#define FL_STD_TEST_ALLOC_HEADER(a) (((struct StdTestAllocation*)a)-1)
#define FL_STD_TEST_ALLOC_DATA(h) (((struct StdTestAllocation*)h) + 1)

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

flut_define_test(std_scoped_resources) {
    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope under normal flow execution") {
        
        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_explain(assert->null(allocate(sizeof(void*))), "Allocation is expected to fail");
                   // End res10
                 }

                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 // End res9
                }

                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                // End res8
               }

               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               // End res7
              }

              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              // End res6
             }

             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             // End res5
            }

            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            // End res4
           }

           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           // End res3
          }

          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          // End res2
         }

         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         // End res1
        }

        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");
    }
    end();

    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope using break (WEIRD BUT EXPECTED)") {
        
        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_explain(assert->null(allocate(sizeof(void*))), "Allocation is expected to fail");
                   break;
                   // End res10
                 }

                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 break;
                 // End res9
                }

                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                break;
                // End res8
               }

               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               break;
               // End res7
              }

              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              break;
              // End res6
             }

             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             break;
             // End res5
            }

            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            break;
            // End res4
           }

           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           break;
           // End res3
          }

          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          break;
          // End res2
         }

         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         break;
         // End res1
        }

        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");
        break;
    }
    end();

    init();
    flut_describe("Scoped resources must be deallocated when exiting the scope using continue (UNEXPECTED)") {
        
        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");

        fl_scoped_resource(void *res1 = allocate(sizeof(void*)), deallocate(res1))
        {
         // Scope res1
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         
         fl_scoped_resource(void *res2 = allocate(sizeof(void*)), deallocate(res2))
         {
          // Scope res2
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          
          fl_scoped_resource(void *res3 = allocate(sizeof(void*)), deallocate(res3))
          {
           // Scope res3
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           
           fl_scoped_resource(void *res4 = allocate(sizeof(void*)), deallocate(res4))
           {
            // Scope res4
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            
            fl_scoped_resource(void *res5 = allocate(sizeof(void*)), deallocate(res5))
            {
             // Scope res5
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             
             fl_scoped_resource(void *res6 = allocate(sizeof(void*)), deallocate(res6))
             {
              // Scope res6
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              
              fl_scoped_resource(void *res7 = allocate(sizeof(void*)), deallocate(res7))
              {
               // Scope res7
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               
               fl_scoped_resource(void *res8 = allocate(sizeof(void*)), deallocate(res8))
               {
                // Scope res8
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                
                fl_scoped_resource(void *res9 = allocate(sizeof(void*)), deallocate(res9))
                {
                 // Scope res9
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                  
                 fl_scoped_resource(void *res10 = allocate(sizeof(void*)), deallocate(res10))
                 {
                   // Scope res10
                   flut_assert_explain(assert->null(allocate(sizeof(void*))), "Allocation is expected to fail");
                   continue;
                   // End res10
                 }

                 flut_assert_explain(assert->is_false(is_allocated(9)), "Slot 10 must be deallocated");
                 flut_assert_explain(assert->is_true(is_allocated(8)), "Slot 9 must be allocated");
                 continue;
                 // End res9
                }

                flut_assert_explain(assert->is_false(is_allocated(8)), "Slot 9 must be deallocated");
                flut_assert_explain(assert->is_true(is_allocated(7)), "Slot 8 must be allocated");
                continue;
                // End res8
               }

               flut_assert_explain(assert->is_false(is_allocated(7)), "Slot 8 must be deallocated");
               flut_assert_explain(assert->is_true(is_allocated(6)), "Slot 7 must be allocated");
               continue;
               // End res7
              }

              flut_assert_explain(assert->is_false(is_allocated(6)), "Slot 7 must be deallocated");
              flut_assert_explain(assert->is_true(is_allocated(5)), "Slot 6 must be allocated");
              continue;
              // End res6
             }

             flut_assert_explain(assert->is_false(is_allocated(5)), "Slot 6 must be deallocated");
             flut_assert_explain(assert->is_true(is_allocated(4)), "Slot 5 must be allocated");
             continue;
             // End res5
            }

            flut_assert_explain(assert->is_false(is_allocated(4)), "Slot 5 must be deallocated");
            flut_assert_explain(assert->is_true(is_allocated(3)), "Slot 4 must be allocated");
            continue;
            // End res4
           }

           flut_assert_explain(assert->is_false(is_allocated(3)), "Slot 4 must be deallocated");
           flut_assert_explain(assert->is_true(is_allocated(2)), "Slot 3 must be allocated");
           continue;
           // End res3
          }

          flut_assert_explain(assert->is_false(is_allocated(2)), "Slot 3 must be deallocated");
          flut_assert_explain(assert->is_true(is_allocated(1)), "Slot 2 must be allocated");
          continue;
          // End res2
         }

         flut_assert_explain(assert->is_false(is_allocated(1)), "Slot 2 must be deallocated");
         flut_assert_explain(assert->is_true(is_allocated(0)), "Slot 1 must be allocated");
         continue;
         // End res1
        }

        flut_assert_explain(assert->is_false(is_allocated(0)), "Slot 1 must be deallocated");
        continue;
    }
    end();
}
