#include "flut.h"
#include <ctype.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <stddef.h>


static bool should_run_suite(struct FlutSuite *suite, int argc, char **argv)
{
    if (argc == 1)
        return true;

    if (argc == 2 && flm_cstring_equals("all", argv[1]))
        return true;

    size_t suite_name_length = strlen(suite->id);
    for (int j = 1; j < argc; j++) {
        size_t arg_length = strlen(argv[j]);

        if (arg_length != suite_name_length)
            continue;

        for (size_t k = 0; k < suite_name_length; k++) {
            if (tolower(suite->id[k]) != tolower(argv[j][k]))
                break;

            if (k == suite_name_length - 1)
                return true;
        }
    }

    return false;
}

void flut_run(int argc, char **argv, FlutSuite **suites, size_t length)
{
    // Compatibility for fl_run macro with a a null-terminator
    if (suites[length - 1] == NULL)
        length--;

    struct FlutSuiteResult *results = fl_array_new(sizeof(struct FlutSuiteResult), length);

    bool should_run = false;
    unsigned long long total_elapsed_time = 0;
    for (size_t i = 0; i < length; i++) {
        results[i].suite = suites[i];
        results[i].ran = false;

        if (!should_run_suite(suites[i], argc, argv))
            continue;

        should_run = true;
        flut_suite_run(suites[i], &results[i]);
        total_elapsed_time += results[i].elapsed;

        printf("\n");
    }

    if (!should_run) {
        fl_array_free(results);
        printf("No tests have been run.\n");
        return;
    }

    size_t ntests = 0;
    size_t nptests = 0;
    printf("+------------------------------------+--------------+------------+------------+\n");
    printf("| %-35s| %-12s | %-10s | %-10s |\n", "Suite", "Passed/Total", "Percentage", "Time (ms)");
    printf("+------------------------------------+--------------+------------+------------+\n");
    for (size_t i = 0; i < length; i++) {
        if (!results[i].ran)
            continue;

        ntests += fl_array_length(suites[i]->tests);
        nptests += results[i].passedTests;
        char *suite_descr = flm_cstring_equals(suites[i]->id, suites[i]->description)
                              ? fl_cstring_dup(suites[i]->id)
                              : fl_cstring_vdup("%s - %s", suites[i]->id, suites[i]->description);
        printf("| %-35s| %6zu/%-5zu | %-2s%3.2f%%%-1s | %7ld ms |\n",
               suite_descr,
               results[i].passedTests,
               fl_array_length(suites[i]->tests),
               "",
               (results[i].passedTests / (float)fl_array_length(suites[i]->tests)) * 100,
               "",
               results[i].elapsed);
        fl_cstring_free(suite_descr);
    }
    printf("+------------------------------------+--------------+------------+------------+\n");
    printf("| %-35s| %6zu/%-5zu | %-2s%3.2f%%%-1s | %7llu ms |\n",
           "Total",
           nptests,
           ntests,
           "",
           (nptests / (float)ntests) * 100,
           "",
           total_elapsed_time);
    printf("+------------------------------------+--------------+------------+------------+\n");

    fl_array_free(results);
}
