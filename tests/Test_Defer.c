#include <fllib.h>
#include "Test.h"
#include "Test_Defer.h"

void test_defer_scope(void)
{
    volatile int numbers[9] = {0};
    defer_scope {
        numbers[0] = 0;
        numbers[1] = 1;
        numbers[2] = 2;
        numbers[3] = 3;
        numbers[4] = 4;
        numbers[5] = 5;    
        numbers[6] = 6;
        numbers[7] = 7;
        numbers[8] = 8;
    }
    
    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}

void test_defer_expressions(void)
{
    volatile int numbers[9] = {0};
    defer_scope {
        defer_expression(numbers[0] = 0);
        defer_expression(numbers[1] = 1);
        defer_expression(numbers[2] = 2);
        defer_expression(numbers[3] = 3);
        defer_expression(numbers[4] = 4);
        defer_expression(numbers[5] = 5);    
        defer_expression(numbers[6] = 6);
        defer_expression(numbers[7] = 7);
        defer_expression(numbers[8] = 8);
    }
    
    for (int i=8; i >= 0; i--)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}

void test_defer_expression_safety(void)
{
    volatile int number = 0;
    defer_scope {
        for (volatile int i=0; i < 10; i++)
        {
            defer_expression(number = i);
        }
    }
    fl_expect("Number must be equals to 10", number == 10);
}

void test_defer_statements(void)
{
    volatile int numbers[9] = {0};
    defer_scope {
        defer_statements {
            for (int i=0; i < 9; i++)
                numbers[i] = i;
        }
    }

    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);

    for (int i=0; i < 9; i++)
        numbers[i] = 0;

    defer_scope {

        defer_statements {
            numbers[5] = 5;
            numbers[6] = 6;
            numbers[7] = 7;
            numbers[8] = 8;
        }

        defer_statements {
            numbers[2] = 2;
            numbers[3] = 3;
            numbers[4] = 4;
        }

        numbers[0] = 0;
        numbers[1] = 1;
    }
    
    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}

void test_defer_stmts_and_exprs(void)
{
    volatile int numbers[9] = {0};
    defer_scope {
        defer_expression(
            numbers[8] = 8
        );
        defer_expression(
            numbers[7] = 7
        );
        defer_expression(
            numbers[6] = 6
        );
        defer_statements {
            numbers[3] = 3;
            numbers[4] = 4;
            numbers[5] = 5;
        }
        numbers[0] = 0;
        numbers[1] = 1;
        numbers[2] = 2;
    }
    
    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}

void test_defer_nested_blocks(void)
{
    volatile int numbers[9] = {0};
    volatile int index = 0;
    defer_scope {
        defer_expression(numbers[8] = index++);
        defer_expression(numbers[7] = index++);
        defer_scope {
            defer_scope {
                defer_expression(numbers[3] = index++);
                defer_statements {
                    numbers[0] = index++;
                    numbers[1] = index++;
                    numbers[2] = index++;
                }
            }
            numbers[4] = index++;
            numbers[5] = index++;
            numbers[6] = index++;
        }
    }
    
    fl_expect("Index must be equals to 9", index == 9);

    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}


void test_defer_nested_blocks_break(void)
{
    volatile int numbers[9] = {0};
    volatile int index = 0;
    defer_scope {
        defer_expression(numbers[8] = index++);
        defer_expression(numbers[7] = index++);
        defer_scope {
            defer_scope {
                defer_expression(numbers[3] = index++);
                defer_statements {
                    numbers[0] = index++;
                    numbers[1] = index++;
                    numbers[2] = index++;
                }
            }

            defer_break();

            numbers[4] = index++;
            numbers[5] = index++;
            numbers[6] = index++;
        }

        numbers[4] = index++;
        numbers[5] = index++;
        numbers[6] = index++;
    }
    
    fl_expect("Index must be equals to 9", index == 9);

    for (int i=0; i < 9; i++)
        fl_vexpect(i == numbers[i], "Numbers array is ordered (%d == %d)", i, numbers[i]);
}

void test_defer_break(void)
{
    char *conststrs[] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven"
    };
    char * volatile strings[8] = { 0 };
    
    defer_scope {
        defer_expression(strings[6] = conststrs[6]);
        
        defer_expression(strings[7] = conststrs[7]);
        
        defer_statements {
            strings[4] = conststrs[4];
            strings[5] = conststrs[5];
        }

        strings[0] = conststrs[0];
        strings[1] = conststrs[1];
        strings[2] = conststrs[2];

        defer_break();

        strings[3] = conststrs[3];
    }

    for (int i=0; i < 8; i++)
    {
        if (i != 3)
            fl_vexpect(strings[i] == conststrs[i], "Strings array in index %d is equals to conststrs[%d] = \"%s\" (\"%s\")", i, i, conststrs[i], strings[i]);
        else
            fl_vexpect(strings[i] == NULL, "Strings array in index %d is NULL", i, i, conststrs[i]);
    }
}

char** defer_return_with_value(char **conststrs)
{
    char **strings = fl_array_new(sizeof(char*), 8);
    
    defer_scope {
        defer_expression(strings[6] = conststrs[6]);
        
        defer_expression(strings[7] = conststrs[7]);
        
        defer_statements {
            strings[4] = conststrs[4];
            strings[5] = conststrs[5];
        }

        strings[0] = conststrs[0];
        strings[1] = conststrs[1];
        strings[2] = conststrs[2];

        defer_return strings;

        strings[3] = conststrs[3];
    }

    return NULL;
}

void defer_return_without_value(char **strings, char **conststrs)
{
    defer_scope {
        defer_expression(strings[6] = conststrs[6]);
        
        defer_expression(strings[7] = conststrs[7]);
        
        defer_statements {
            strings[4] = conststrs[4];
            strings[5] = conststrs[5];
        }

        strings[0] = conststrs[0];
        strings[1] = conststrs[1];
        strings[2] = conststrs[2];

        defer_return;

        strings[3] = conststrs[3];
    }
}

void test_defer_return(void)
{
    char *conststrs[] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven"
    };
    defer_scope {
        char **strings = fl_array_new(sizeof(char*), 8);
        defer_expression(fl_array_delete(strings));
        
        defer_return_without_value(strings, conststrs);

        for (int i=0; i < 8; i++)
        {
            if (i != 3)
                fl_vexpect(strings[i] == conststrs[i], "Strings array in index %d is equals to conststrs[%d] = \"%s\"", i, i, conststrs[i]);
            else
                fl_vexpect(strings[i] == NULL, "Strings array in index %d is NULL", i, i, conststrs[i]);
        }
    }    

    defer_scope {
        char **strings = defer_return_with_value(conststrs);
        defer_expression(fl_array_delete(strings));

        fl_vexpect(strings != NULL, "Return from deferred scope must be a valid array");

        for (int i=0; i < 8; i++)
        {
            if (i != 3)
                fl_vexpect(strings[i] == conststrs[i], "Strings array in index %d is equals to conststrs[%d] = \"%s\"", i, i, conststrs[i]);
            else
                fl_vexpect(strings[i] == NULL, "Strings array in index %d is NULL", i, i, conststrs[i]);
        }   
    }
}

void test_defer_file(void)
{
    defer_scope {
        FILE *fd = NULL;
        char *path = NULL;

        #if defined(_WIN32) && __STDC_WANT_SECURE_LIB__
        path = "c:\\tmp\\defer_file.txt";
        errno_t fdres = fopen_s(&fd, path, "w+");
        fl_vexpect(fdres == 0 && fd, "fopen call to open %s must return a valid FILE*", path);
        #else
        path = "/tmp/defer_file.txt";
        fd = fopen(path, "w+");
        fl_vexpect(fd != NULL, "fopen call to open %s must return a valid FILE*", path);
        #endif
    
        defer_expression(fl_expect("fclose call must return 0", fclose(fd) == 0));

        fl_expect("Bytes written must be greater than 0", fwrite("test", 1, 4, fd) > 0);
        
        fflush(fd);
        fseek(fd, 0, 0);

        char buf[5] = {0};
        fl_expect("Bytes read must be greater than 0", fread(buf, 1, 4, fd) > 0 );

        fl_vexpect(flm_cstring_equals_n("test", buf, 4), "Text read from file %s must be equals to 'test'", path);
    }
}
