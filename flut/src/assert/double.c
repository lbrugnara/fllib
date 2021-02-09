#include <stddef.h>
#include <math.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "double.h"

FlutAssertResult* flut__assert_double_equals(double expected, double actual, double epsilon)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = fabs(expected - actual) <= epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be equals to %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_double_not_equals(double expected, double actual, double epsilon)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = fabs(expected - actual) > epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be not-equals to %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_double_gt(double expected, double actual, double epsilon) {
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = actual - expected > epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be greater than %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_double_gte(double expected, double actual, double epsilon) {
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = actual - expected >= epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be greater than or equals to %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_double_lt(double expected, double actual, double epsilon) {
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected - actual > epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be lesser than %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_double_lte(double expected, double actual, double epsilon) {
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected - actual >= epsilon;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting double value to be lesser than or equals to %f (with an acceptable error of %f), actually %f", expected, epsilon, actual);
    }

    return result;
}
