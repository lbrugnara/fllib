#!/bin/bash
set -x
mkdir -p obj/linux/x86/gcc/clang-debug/src/containers obj/linux/x86/gcc/clang-debug/src/os obj/linux/x86/gcc/clang-debug/src/text obj/linux/x86/gcc/clang-debug/src/threading
mkdir -p obj/linux/x86/gcc/clang-debug/tests/containers obj/linux/x86/gcc/clang-debug/tests/text
mkdir -p obj/linux/x86/gcc/clang-debug/flut/src/assert
mkdir -p build/linux/x86/gcc/clang-debug
mkdir -p ./include/fllib
mkdir -p ./include/flut
(cd ./src && find . -type f -name '*.h' | xargs cp --parents -t ../include/fllib)
(cd ./flut/src && find . -type f -name '*.h' | xargs cp --parents -t ../../include/flut)
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/BinaryHeap.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Container.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Container.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Hashtable.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Hashtable.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Iterator.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Iterator.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/List.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/List.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Vector.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Vector.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Process.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Process.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Signal.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Signal.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/System.c -o obj/linux/x86_64/gcc/clang-debug/src/os/System.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Timer.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Timer.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/Regex.c -o obj/linux/x86_64/gcc/clang-debug/src/text/Regex.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/String.c -o obj/linux/x86_64/gcc/clang-debug/src/text/String.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/Unicode.c -o obj/linux/x86_64/gcc/clang-debug/src/text/Unicode.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/UnicodeData.c -o obj/linux/x86_64/gcc/clang-debug/src/text/UnicodeData.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/threading/Thread.c -o obj/linux/x86_64/gcc/clang-debug/src/threading/Thread.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Array.c -o obj/linux/x86_64/gcc/clang-debug/src/Array.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Cstring.c -o obj/linux/x86_64/gcc/clang-debug/src/Cstring.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Error.c -o obj/linux/x86_64/gcc/clang-debug/src/Error.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/IO.c -o obj/linux/x86_64/gcc/clang-debug/src/IO.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Mem.c -o obj/linux/x86_64/gcc/clang-debug/src/Mem.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Slice.c -o obj/linux/x86_64/gcc/clang-debug/src/Slice.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Std.c -o obj/linux/x86_64/gcc/clang-debug/src/Std.o 
ar rcs build/linux/x86_64/gcc/clang-debug/libfl.a  obj/linux/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o obj/linux/x86_64/gcc/clang-debug/src/containers/Container.o obj/linux/x86_64/gcc/clang-debug/src/containers/Hashtable.o obj/linux/x86_64/gcc/clang-debug/src/containers/Iterator.o obj/linux/x86_64/gcc/clang-debug/src/containers/List.o obj/linux/x86_64/gcc/clang-debug/src/containers/Vector.o obj/linux/x86_64/gcc/clang-debug/src/os/Process.o obj/linux/x86_64/gcc/clang-debug/src/os/Signal.o obj/linux/x86_64/gcc/clang-debug/src/os/System.o obj/linux/x86_64/gcc/clang-debug/src/os/Timer.o obj/linux/x86_64/gcc/clang-debug/src/text/Regex.o obj/linux/x86_64/gcc/clang-debug/src/text/String.o obj/linux/x86_64/gcc/clang-debug/src/text/Unicode.o obj/linux/x86_64/gcc/clang-debug/src/text/UnicodeData.o obj/linux/x86_64/gcc/clang-debug/src/threading/Thread.o obj/linux/x86_64/gcc/clang-debug/src/Array.o obj/linux/x86_64/gcc/clang-debug/src/Cstring.o obj/linux/x86_64/gcc/clang-debug/src/Error.o obj/linux/x86_64/gcc/clang-debug/src/IO.o obj/linux/x86_64/gcc/clang-debug/src/Mem.o obj/linux/x86_64/gcc/clang-debug/src/Slice.o obj/linux/x86_64/gcc/clang-debug/src/Std.o
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/containers/Test_BinaryHeap.c -o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_BinaryHeap.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/containers/Test_Hashtable.c -o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_Hashtable.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/containers/Test_List.c -o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_List.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/containers/Test_Vector.c -o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_Vector.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/IO.c -o obj/linux/x86_64/gcc/clang-debug/tests/IO.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/Main.c -o obj/linux/x86_64/gcc/clang-debug/tests/Main.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/Test_Array.c -o obj/linux/x86_64/gcc/clang-debug/tests/Test_Array.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/Test_Cstring.c -o obj/linux/x86_64/gcc/clang-debug/tests/Test_Cstring.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/Test_Slice.c -o obj/linux/x86_64/gcc/clang-debug/tests/Test_Slice.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/Test_Std.c -o obj/linux/x86_64/gcc/clang-debug/tests/Test_Std.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/text/Test_Regex.c -o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_Regex.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/text/Test_String.c -o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_String.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./tests/text/Test_Unicode.c -o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_Unicode.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/BinaryHeap.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Container.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Container.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Hashtable.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Hashtable.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Iterator.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Iterator.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/List.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/List.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/containers/Vector.c -o obj/linux/x86_64/gcc/clang-debug/src/containers/Vector.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Process.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Process.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Signal.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Signal.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/System.c -o obj/linux/x86_64/gcc/clang-debug/src/os/System.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/os/Timer.c -o obj/linux/x86_64/gcc/clang-debug/src/os/Timer.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/Regex.c -o obj/linux/x86_64/gcc/clang-debug/src/text/Regex.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/String.c -o obj/linux/x86_64/gcc/clang-debug/src/text/String.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/Unicode.c -o obj/linux/x86_64/gcc/clang-debug/src/text/Unicode.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/text/UnicodeData.c -o obj/linux/x86_64/gcc/clang-debug/src/text/UnicodeData.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/threading/Thread.c -o obj/linux/x86_64/gcc/clang-debug/src/threading/Thread.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Array.c -o obj/linux/x86_64/gcc/clang-debug/src/Array.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Cstring.c -o obj/linux/x86_64/gcc/clang-debug/src/Cstring.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Error.c -o obj/linux/x86_64/gcc/clang-debug/src/Error.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/IO.c -o obj/linux/x86_64/gcc/clang-debug/src/IO.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Mem.c -o obj/linux/x86_64/gcc/clang-debug/src/Mem.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Slice.c -o obj/linux/x86_64/gcc/clang-debug/src/Slice.o 
gcc   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./src/Std.c -o obj/linux/x86_64/gcc/clang-debug/src/Std.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert/bool.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/bool.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert/null.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/null.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert/result.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/result.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert/size_t.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/size_t.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert/str.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/str.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/assert.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/assert.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/context.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/context.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/expect.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/expect.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/flut.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/flut.o 
gcc  -I./include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=gnu99 -fPIC -Wno-nonnull -m64 -ggdb -O0 -c ./flut/src/suite.c -o obj/linux/x86_64/gcc/clang-debug/flut/src/suite.o 
ar rcs build/linux/x86_64/gcc/clang-debug/libflut.a  obj/linux/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o obj/linux/x86_64/gcc/clang-debug/src/containers/Container.o obj/linux/x86_64/gcc/clang-debug/src/containers/Hashtable.o obj/linux/x86_64/gcc/clang-debug/src/containers/Iterator.o obj/linux/x86_64/gcc/clang-debug/src/containers/List.o obj/linux/x86_64/gcc/clang-debug/src/containers/Vector.o obj/linux/x86_64/gcc/clang-debug/src/os/Process.o obj/linux/x86_64/gcc/clang-debug/src/os/Signal.o obj/linux/x86_64/gcc/clang-debug/src/os/System.o obj/linux/x86_64/gcc/clang-debug/src/os/Timer.o obj/linux/x86_64/gcc/clang-debug/src/text/Regex.o obj/linux/x86_64/gcc/clang-debug/src/text/String.o obj/linux/x86_64/gcc/clang-debug/src/text/Unicode.o obj/linux/x86_64/gcc/clang-debug/src/text/UnicodeData.o obj/linux/x86_64/gcc/clang-debug/src/threading/Thread.o obj/linux/x86_64/gcc/clang-debug/src/Array.o obj/linux/x86_64/gcc/clang-debug/src/Cstring.o obj/linux/x86_64/gcc/clang-debug/src/Error.o obj/linux/x86_64/gcc/clang-debug/src/IO.o obj/linux/x86_64/gcc/clang-debug/src/Mem.o obj/linux/x86_64/gcc/clang-debug/src/Slice.o obj/linux/x86_64/gcc/clang-debug/src/Std.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/bool.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/null.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/result.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/size_t.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert/str.o obj/linux/x86_64/gcc/clang-debug/flut/src/assert.o obj/linux/x86_64/gcc/clang-debug/flut/src/context.o obj/linux/x86_64/gcc/clang-debug/flut/src/expect.o obj/linux/x86_64/gcc/clang-debug/flut/src/flut.o obj/linux/x86_64/gcc/clang-debug/flut/src/suite.o
gcc -m64 -o build/linux/x86_64/gcc/clang-debug/tests   obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_BinaryHeap.o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_Hashtable.o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_List.o obj/linux/x86_64/gcc/clang-debug/tests/containers/Test_Vector.o obj/linux/x86_64/gcc/clang-debug/tests/IO.o obj/linux/x86_64/gcc/clang-debug/tests/Main.o obj/linux/x86_64/gcc/clang-debug/tests/Test_Array.o obj/linux/x86_64/gcc/clang-debug/tests/Test_Cstring.o obj/linux/x86_64/gcc/clang-debug/tests/Test_Slice.o obj/linux/x86_64/gcc/clang-debug/tests/Test_Std.o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_Regex.o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_String.o obj/linux/x86_64/gcc/clang-debug/tests/text/Test_Unicode.o build/linux/x86_64/gcc/clang-debug/libflut.a -lm -lpthread 
