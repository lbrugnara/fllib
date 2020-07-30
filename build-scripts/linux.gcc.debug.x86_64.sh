#!/bin/bash
set -x
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/BinaryHeap.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Container.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Container.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Hashtable.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Hashtable.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Iterator.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Iterator.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/List.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/List.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Vector.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Vector.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Process.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Process.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Signal.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Signal.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/System.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/System.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Timer.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Timer.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/Regex.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Regex.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/String.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/String.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/Unicode.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Unicode.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/UnicodeData.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/UnicodeData.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/threading/Thread.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/threading/Thread.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Array.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Array.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Cstring.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Cstring.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Error.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Error.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/IO.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/IO.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Mem.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Mem.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Slice.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Slice.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Std.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Std.o 
ar rcs build/linux-bash/x86_64/gcc/clang-debug/libfl.a  obj/linux-bash/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Container.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Hashtable.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Iterator.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/List.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Vector.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Process.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Signal.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/System.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Timer.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Regex.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/String.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Unicode.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/UnicodeData.o obj/linux-bash/x86_64/gcc/clang-debug/src/threading/Thread.o obj/linux-bash/x86_64/gcc/clang-debug/src/Array.o obj/linux-bash/x86_64/gcc/clang-debug/src/Cstring.o obj/linux-bash/x86_64/gcc/clang-debug/src/Error.o obj/linux-bash/x86_64/gcc/clang-debug/src/IO.o obj/linux-bash/x86_64/gcc/clang-debug/src/Mem.o obj/linux-bash/x86_64/gcc/clang-debug/src/Slice.o obj/linux-bash/x86_64/gcc/clang-debug/src/Std.o
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/containers/Test_BinaryHeap.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_BinaryHeap.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/containers/Test_Hashtable.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_Hashtable.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/containers/Test_List.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_List.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/containers/Test_Vector.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_Vector.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/IO.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/IO.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/Main.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/Main.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/Std.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/Std.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/Test_Array.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Array.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/Test_Cstring.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Cstring.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/Test_Slice.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Slice.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/text/Test_Regex.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_Regex.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/text/Test_String.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_String.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./tests/text/Test_Unicode.c -o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_Unicode.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/BinaryHeap.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Container.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Container.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Hashtable.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Hashtable.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Iterator.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Iterator.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/List.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/List.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/containers/Vector.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Vector.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Process.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Process.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Signal.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Signal.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/System.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/System.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/os/Timer.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Timer.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/Regex.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Regex.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/String.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/String.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/Unicode.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Unicode.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/text/UnicodeData.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/text/UnicodeData.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/threading/Thread.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/threading/Thread.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Array.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Array.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Cstring.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Cstring.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Error.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Error.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/IO.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/IO.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Mem.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Mem.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Slice.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Slice.o 
gcc   -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./src/Std.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/Std.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert/bool.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/bool.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert/null.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/null.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert/result.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/result.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert/size_t.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/size_t.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert/str.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/str.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/assert.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/context.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/context.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/expect.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/expect.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/flut.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/flut.o 
gcc  -I./include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -std=gnu99 -ggdb -O0 -DFL_DEBUG -c ./flut/src/suite.c -o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/suite.o 
ar rcs build/linux-bash/x86_64/gcc/clang-debug/libflut.a  obj/linux-bash/x86_64/gcc/clang-debug/src/containers/BinaryHeap.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Container.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Hashtable.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Iterator.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/List.o obj/linux-bash/x86_64/gcc/clang-debug/src/containers/Vector.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Process.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Signal.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/System.o obj/linux-bash/x86_64/gcc/clang-debug/src/os/Timer.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Regex.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/String.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/Unicode.o obj/linux-bash/x86_64/gcc/clang-debug/src/text/UnicodeData.o obj/linux-bash/x86_64/gcc/clang-debug/src/threading/Thread.o obj/linux-bash/x86_64/gcc/clang-debug/src/Array.o obj/linux-bash/x86_64/gcc/clang-debug/src/Cstring.o obj/linux-bash/x86_64/gcc/clang-debug/src/Error.o obj/linux-bash/x86_64/gcc/clang-debug/src/IO.o obj/linux-bash/x86_64/gcc/clang-debug/src/Mem.o obj/linux-bash/x86_64/gcc/clang-debug/src/Slice.o obj/linux-bash/x86_64/gcc/clang-debug/src/Std.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/bool.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/null.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/result.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/size_t.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert/str.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/assert.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/context.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/expect.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/flut.o obj/linux-bash/x86_64/gcc/clang-debug/flut/src/suite.o
gcc -o build/linux-bash/x86_64/gcc/clang-debug/tests   obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_BinaryHeap.o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_Hashtable.o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_List.o obj/linux-bash/x86_64/gcc/clang-debug/tests/containers/Test_Vector.o obj/linux-bash/x86_64/gcc/clang-debug/tests/IO.o obj/linux-bash/x86_64/gcc/clang-debug/tests/Main.o obj/linux-bash/x86_64/gcc/clang-debug/tests/Std.o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Array.o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Cstring.o obj/linux-bash/x86_64/gcc/clang-debug/tests/Test_Slice.o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_Regex.o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_String.o obj/linux-bash/x86_64/gcc/clang-debug/tests/text/Test_Unicode.o build/linux-bash/x86_64/gcc/clang-debug/libflut.a -lm -lpthread 
