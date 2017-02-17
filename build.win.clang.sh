#!/bin/bash

set -x

mkdir -p ./obj/debug
mkdir -p ./obj/debug/threading
mkdir -p ./obj/debug/containers
mkdir -p ./obj/debug/os
mkdir -p ./obj/debug/text

CFLAGS="-O3 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=c99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8"

clang $CFLAGS -c ./src/threading/Thread.c  -o obj/debug/threading/Thread.o
clang $CFLAGS -c ./src/Std.c  -o obj/debug/Std.o
clang $CFLAGS -c ./src/Error.c  -o obj/debug/Error.o
clang $CFLAGS -c ./src/Mem.c  -o obj/debug/Mem.o
clang $CFLAGS -c ./src/text/Unicode.c  -o obj/debug/text/Unicode.o
clang $CFLAGS -c ./src/text/String.c  -o obj/debug/text/String.o
clang $CFLAGS -c ./src/Cstr.c  -o obj/debug/Cstr.o
clang $CFLAGS -c ./src/Array.c  -o obj/debug/Array.o
clang $CFLAGS -c ./src/File.c  -o obj/debug/File.o
clang $CFLAGS -c ./src/containers/Iterator.c  -o obj/debug/containers/Iterator.o
clang $CFLAGS -c ./src/containers/Vector.c  -o obj/debug/containers/Vector.o
clang $CFLAGS -c ./src/containers/List.c  -o obj/debug/containers/List.o
clang $CFLAGS -c ./src/containers/KeyValuePair.c  -o obj/debug/containers/KeyValuePair.o
clang $CFLAGS -c ./src/containers/Dictionary.c  -o obj/debug/containers/Dictionary.o
clang $CFLAGS -c ./src/text/Regex.c  -o obj/debug/text/Regex.o
clang $CFLAGS -c ./src/os/System.c  -o obj/debug/os/System.o
clang $CFLAGS -c ./src/os/Signal.c  -o obj/debug/os/Signal.o
clang $CFLAGS -c ./src/os/WinEx.c -o obj/debug/os/WinEx.o

mkdir -p ./build/debug

lib -NOLOGO -VERBOSE                        \
    obj/debug/threading/Thread.o            \
    obj/debug/Std.o                         \
    obj/debug/Error.o                       \
    obj/debug/Mem.o                         \
    obj/debug/text/Unicode.o                \
    obj/debug/text/String.o                 \
    obj/debug/Cstr.o                        \
    obj/debug/Array.o                       \
    obj/debug/File.o                        \
    obj/debug/containers/Iterator.o         \
    obj/debug/containers/Vector.o           \
    obj/debug/containers/List.o             \
    obj/debug/containers/KeyValuePair.o     \
    obj/debug/containers/Dictionary.o       \
    obj/debug/text/Regex.o                  \
    obj/debug/os/System.o                   \
    obj/debug/os/Signal.o                   \
    obj/debug/os/WinEx.o                    \
    -OUT:build/debug/libfl.lib

# Tests

mkdir -p ./tests/obj/debug/{threading,text}

clang $CFLAGS -I./include -I./src -c tests/Main.c -o tests/obj/debug/Main.o
clang $CFLAGS -I./include -I./src -c tests/Test.c -o tests/obj/debug/Test.o
clang $CFLAGS -I./include -I./src -c tests/Std.c -o tests/obj/debug/Std.o
clang $CFLAGS -I./include -I./src -c tests/File.c -o tests/obj/debug/File.o
clang $CFLAGS -I./include -I./src -c tests/Cstr.c -o tests/obj/debug/Cstr.o
clang $CFLAGS -I./include -I./src -c tests/text/Test_Unicode.c -o tests/obj/debug/text/Test_Unicode.o
clang $CFLAGS -I./include -I./src -c tests/text/Test_String.c -o tests/obj/debug/text/Test_String.o

mkdir -p ./tests/build/debug

clang $CFLAGS -I./include -I./src -o tests/build/debug/tests.exe \
    build/debug/libfl.lib   \
    tests/obj/debug/Main.o  \
    tests/obj/debug/Test.o  \
    tests/obj/debug/Std.o   \
    tests/obj/debug/File.o  \
    tests/obj/debug/text/Test_Unicode.o  \
    tests/obj/debug/text/Test_String.o  \
    tests/obj/debug/Cstr.o