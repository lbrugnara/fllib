#ifndef FLUT_MAP_H
#define FLUT_MAP_H

#define FLUT_DO_PP(x) x

#define FLUT_MAP_1(mapfn, x)        mapfn(x)
#define FLUT_MAP_2(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_1(mapfn,  __VA_ARGS__))
#define FLUT_MAP_3(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_2(mapfn, __VA_ARGS__))
#define FLUT_MAP_4(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_3(mapfn,  __VA_ARGS__))
#define FLUT_MAP_5(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_4(mapfn,  __VA_ARGS__))
#define FLUT_MAP_6(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_5(mapfn,  __VA_ARGS__))
#define FLUT_MAP_7(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_6(mapfn,  __VA_ARGS__))
#define FLUT_MAP_8(mapfn, x, ...)   mapfn(x); FLUT_DO_PP(FLUT_MAP_7(mapfn,  __VA_ARGS__))

#define FLUT_MAP_ARG_1(mapfn, arg, x)       mapfn(arg, x)
#define FLUT_MAP_ARG_2(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_1(mapfn, arg, __VA_ARGS__))
#define FLUT_MAP_ARG_3(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_2(mapfn, arg, __VA_ARGS__))
#define FLUT_MAP_ARG_4(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_3(mapfn, arg,  __VA_ARGS__))
#define FLUT_MAP_ARG_5(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_4(mapfn, arg,  __VA_ARGS__))
#define FLUT_MAP_ARG_6(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_5(mapfn, arg,  __VA_ARGS__))
#define FLUT_MAP_ARG_7(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_6(mapfn, arg,  __VA_ARGS__))
#define FLUT_MAP_ARG_8(mapfn, arg, x, ...)  mapfn(arg, x); FLUT_DO_PP(FLUT_MAP_ARG_7(mapfn, arg,  __VA_ARGS__))

#define FLUT_MAP_FILTER_1(mapfn, filter, arg, x)        FLUT_MAP(mapfn, filter(arg, x))
#define FLUT_MAP_FILTER_2(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_1(mapfn, filter, arg,  __VA_ARGS__))
#define FLUT_MAP_FILTER_3(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_2(mapfn, filter, arg, __VA_ARGS__))
#define FLUT_MAP_FILTER_4(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_3(mapfn, filter, arg,  __VA_ARGS__))
#define FLUT_MAP_FILTER_5(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_4(mapfn, filter, arg,  __VA_ARGS__))
#define FLUT_MAP_FILTER_6(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_5(mapfn, filter, arg,  __VA_ARGS__))
#define FLUT_MAP_FILTER_7(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_6(mapfn, filter, arg,  __VA_ARGS__))
#define FLUT_MAP_FILTER_8(mapfn, filter, arg, x, ...)   FLUT_MAP(mapfn, filter(arg, x)); FLUT_DO_PP(FLUT_MAP_FILTER_7(mapfn, filter, arg,  __VA_ARGS__))

#define FLUT_COUNT_ARGS(...) FLUT_COUNT_ARGS_(__VA_ARGS__, FLUT_COUNT_TO_N())
#define FLUT_COUNT_ARGS_(...) FLUT_DO_PP(FLUT_GET_PARAM_AT_N(__VA_ARGS__))
#define FLUT_GET_PARAM_AT_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FLUT_COUNT_TO_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FLUT_CONCAT(x,y) x##y

#define FLUT_MAP_(N, mapfn, ...) FLUT_DO_PP(FLUT_CONCAT(FLUT_MAP_, N)(mapfn, __VA_ARGS__))
#define FLUT_MAP(mapfn, ...) FLUT_MAP_(FLUT_COUNT_ARGS(__VA_ARGS__), mapfn, __VA_ARGS__)

#define FLUT_MAP_ARG_(N, mapfn, arg, ...) FLUT_DO_PP(FLUT_CONCAT(FLUT_MAP_ARG_, N)(mapfn, arg, __VA_ARGS__))
#define FLUT_MAP_ARG(mapfn, arg, ...) FLUT_MAP_ARG_(FLUT_COUNT_ARGS(__VA_ARGS__), mapfn, arg, __VA_ARGS__)

#define FLUT_MAP_FILTER_(N, mapfn, filter, arg, ...) FLUT_DO_PP(FLUT_CONCAT(FLUT_MAP_FILTER_, N)(mapfn, filter, arg, __VA_ARGS__))
#define FLUT_MAP_FILTER(mapfn, filter, arg, ...) FLUT_MAP_FILTER_(FLUT_COUNT_ARGS(__VA_ARGS__), mapfn, filter, arg, __VA_ARGS__)

#endif /* FLUT_MAP_H */
