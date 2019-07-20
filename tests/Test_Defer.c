#include <fllib.h>
#include "Test.h"
#include "Test_Defer.h"

void test_defer_scope(void)
{
    int numbers[9] = {0};
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
    int numbers[9] = {0};
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

void test_defer_statements(void)
{
    int numbers[9] = {0};
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
    int numbers[9] = {0};
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

void test_defer_exit()
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
    char *strings[8] = { 0 };
    
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

        defer_exit();

        strings[3] = conststrs[3];
    }

    for (int i=0; i < 8; i++)
    {
        if (i != 3)
            fl_vexpect(strings[i] == conststrs[i], "Strings array in index %d is equals to conststrs[%d] = \"%s\"", i, i, conststrs[i]);
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

void test_defer_return()
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
