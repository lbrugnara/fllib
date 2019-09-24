#include <fllib.h>

#include "Test_Array.h"
#include "Test.h"

void test_array_combine(void)
{
    {
        int *zero_to_nine = fl_array_new(sizeof(int), 10);
        int *ten_to_nineteen = fl_array_new(sizeof(int), 10);

        for (int i=0; i < 10; i++)
        {
            zero_to_nine[i] = i;
            ten_to_nineteen[i] = i + 10;
        }

        int *zero_to_nineteen = fl_array_new(sizeof(int), 0);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, zero_to_nine);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, ten_to_nineteen);

        fl_expect("Array must contain 20 elements", fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
            fl_vexpect(i == zero_to_nineteen[i], "Element at position %d must be equals to %d", i, i);

        fl_array_delete(zero_to_nine);
        fl_array_delete(ten_to_nineteen);
        fl_array_delete(zero_to_nineteen);
    }

    {
        int numbers[20];

        int **zero_to_nine = fl_array_new(sizeof(int*), 10);
        int **ten_to_nineteen = fl_array_new(sizeof(int*), 10);

        for (int i=0; i < 10; i++)
        {
            numbers[i] = i;
            numbers[i + 10] = i + 10;
            zero_to_nine[i] = numbers + i;
            ten_to_nineteen[i] = numbers + i + 10;
        }

        int **zero_to_nineteen = fl_array_new(sizeof(int*), 0);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, zero_to_nine);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, ten_to_nineteen);

        fl_expect("Array must contain 20 elements", fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
        {
            fl_vexpect(numbers + i == zero_to_nineteen[i], "Element at position %d must point to numbers[%d]", i, i);
            fl_vexpect(i == *zero_to_nineteen[i], "Element at position %d must be equals to %d", i, i);
        }

        fl_array_delete(zero_to_nine);
        fl_array_delete(ten_to_nineteen);
        fl_array_delete(zero_to_nineteen);
    }
}
