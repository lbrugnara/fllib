#include <fllib.h>

#include "Test_Array.h"
#include <flut/flut.h>

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

        flut_expect_compat("Array must contain 20 elements", fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
            flut_vexpect_compat(i == zero_to_nineteen[i], "Element at position %d must be equals to %d", i, i);

        fl_array_free(zero_to_nine);
        fl_array_free(ten_to_nineteen);
        fl_array_free(zero_to_nineteen);
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

        flut_expect_compat("Array must contain 20 elements", fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
        {
            flut_vexpect_compat(numbers + i == zero_to_nineteen[i], "Element at position %d must point to numbers[%d]", i, i);
            flut_vexpect_compat(i == *zero_to_nineteen[i], "Element at position %d must be equals to %d", i, i);
        }

        fl_array_free(zero_to_nine);
        fl_array_free(ten_to_nineteen);
        fl_array_free(zero_to_nineteen);
    }
}

void test_array_append(void)
{
    int *numbers = fl_array_new(sizeof(int), 0);

    for (int i=0; i < 10; i++)
        numbers = fl_array_append(numbers, &i);

    flut_expect_compat("Numbers array must contain 10 elements", fl_array_length(numbers) == 10);

    for (int i=0; i < 10; i++)
            flut_vexpect_compat(i == numbers[i], "Element at position %d must be equals to %d", i, i);

    fl_array_free(numbers);
}
