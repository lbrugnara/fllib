#include <fllib.h>
#include "Test.h"
#include "Test_Defer.h"

void test_defer_scope(void)
{
    int numbers[9] = {0};
    int index = -1;
    defer_scope {
        defer_expression(
            (index++, numbers[index] = index)
        );
        defer_expression(
            (index++, numbers[index] = index)
        );
        defer_expression(
            (index++, numbers[index] = index)
        );
        defer_statements {
            (index++, numbers[index] = index);
            (index++, numbers[index] = index);
            (index++, numbers[index] = index);    
        }
        (index++, numbers[index] = index);
        (index++, numbers[index] = index);
        (index++, numbers[index] = index);
    }
    
    for (int i=0; i < 9; i++)
        flm_vassert(i == numbers[i], "Numbers array is ordered (%d == %d)\n", i, numbers[i]);
}
