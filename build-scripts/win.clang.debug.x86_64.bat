mkdir     obj\win\x86_64\clang\clang-debug\src     obj\win\x86_64\clang\clang-debug\src\containers     obj\win\x86_64\clang\clang-debug\src\os     obj\win\x86_64\clang\clang-debug\src\text     obj\win\x86_64\clang\clang-debug\src\threading 
mkdir     obj\win\x86_64\clang\clang-debug\flut\src     obj\win\x86_64\clang\clang-debug\flut\src\assert 
mkdir     obj\win\x86_64\clang\clang-debug\tests\     obj\win\x86_64\clang\clang-debug\tests\containers     obj\win\x86_64\clang\clang-debug\tests\text  
mkdir build\win\x86_64\clang\clang-debug 
if not exist .\include (mkdir .\include)
if not exist .\include\fllib (mkdir .\include\fllib)
if not exist .\include\flut (mkdir .\include\flut)
xcopy /y /d /s .\src\*.h .\include\fllib
xcopy /y /d /s .\flut\src\*.h .\include\flut
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\BinaryHeap.c -o obj\win\x86_64\clang\clang-debug\src\containers\BinaryHeap.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Container.c -o obj\win\x86_64\clang\clang-debug\src\containers\Container.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Hashtable.c -o obj\win\x86_64\clang\clang-debug\src\containers\Hashtable.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Iterator.c -o obj\win\x86_64\clang\clang-debug\src\containers\Iterator.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\List.c -o obj\win\x86_64\clang\clang-debug\src\containers\List.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Vector.c -o obj\win\x86_64\clang\clang-debug\src\containers\Vector.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Process.c -o obj\win\x86_64\clang\clang-debug\src\os\Process.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Signal.c -o obj\win\x86_64\clang\clang-debug\src\os\Signal.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\System.c -o obj\win\x86_64\clang\clang-debug\src\os\System.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Timer.c -o obj\win\x86_64\clang\clang-debug\src\os\Timer.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\Regex.c -o obj\win\x86_64\clang\clang-debug\src\text\Regex.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\String.c -o obj\win\x86_64\clang\clang-debug\src\text\String.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\Unicode.c -o obj\win\x86_64\clang\clang-debug\src\text\Unicode.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\UnicodeData.c -o obj\win\x86_64\clang\clang-debug\src\text\UnicodeData.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\threading\Thread.c -o obj\win\x86_64\clang\clang-debug\src\threading\Thread.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Array.c -o obj\win\x86_64\clang\clang-debug\src\Array.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Cstring.c -o obj\win\x86_64\clang\clang-debug\src\Cstring.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Error.c -o obj\win\x86_64\clang\clang-debug\src\Error.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\IO.c -o obj\win\x86_64\clang\clang-debug\src\IO.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Mem.c -o obj\win\x86_64\clang\clang-debug\src\Mem.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Slice.c -o obj\win\x86_64\clang\clang-debug\src\Slice.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Std.c -o obj\win\x86_64\clang\clang-debug\src\Std.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c src\os\WinEx.c -o obj\win\x86_64\clang\clang-debug\src\os\WinEx.o 
llvm-lib.exe /NOLOGO /OUT:build\win\x86_64\clang\clang-debug\libfl.lib  obj\win\x86_64\clang\clang-debug\src\containers\BinaryHeap.o obj\win\x86_64\clang\clang-debug\src\containers\Container.o obj\win\x86_64\clang\clang-debug\src\containers\Hashtable.o obj\win\x86_64\clang\clang-debug\src\containers\Iterator.o obj\win\x86_64\clang\clang-debug\src\containers\List.o obj\win\x86_64\clang\clang-debug\src\containers\Vector.o obj\win\x86_64\clang\clang-debug\src\os\Process.o obj\win\x86_64\clang\clang-debug\src\os\Signal.o obj\win\x86_64\clang\clang-debug\src\os\System.o obj\win\x86_64\clang\clang-debug\src\os\Timer.o obj\win\x86_64\clang\clang-debug\src\text\Regex.o obj\win\x86_64\clang\clang-debug\src\text\String.o obj\win\x86_64\clang\clang-debug\src\text\Unicode.o obj\win\x86_64\clang\clang-debug\src\text\UnicodeData.o obj\win\x86_64\clang\clang-debug\src\threading\Thread.o obj\win\x86_64\clang\clang-debug\src\Array.o obj\win\x86_64\clang\clang-debug\src\Cstring.o obj\win\x86_64\clang\clang-debug\src\Error.o obj\win\x86_64\clang\clang-debug\src\IO.o obj\win\x86_64\clang\clang-debug\src\Mem.o obj\win\x86_64\clang\clang-debug\src\Slice.o obj\win\x86_64\clang\clang-debug\src\Std.o obj\win\x86_64\clang\clang-debug\src\os\WinEx.o
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\array.unit.c -o obj\win\x86_64\clang\clang-debug\tests\array.unit.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\containers\Test_BinaryHeap.c -o obj\win\x86_64\clang\clang-debug\tests\containers\Test_BinaryHeap.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\containers\Test_Hashtable.c -o obj\win\x86_64\clang\clang-debug\tests\containers\Test_Hashtable.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\containers\Test_List.c -o obj\win\x86_64\clang\clang-debug\tests\containers\Test_List.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\containers\Test_Vector.c -o obj\win\x86_64\clang\clang-debug\tests\containers\Test_Vector.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\cstring.unit.c -o obj\win\x86_64\clang\clang-debug\tests\cstring.unit.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\IO.c -o obj\win\x86_64\clang\clang-debug\tests\IO.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\main.unit.c -o obj\win\x86_64\clang\clang-debug\tests\main.unit.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\slice.unit.c -o obj\win\x86_64\clang\clang-debug\tests\slice.unit.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\std.unit.c -o obj\win\x86_64\clang\clang-debug\tests\std.unit.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\text\Test_Regex.c -o obj\win\x86_64\clang\clang-debug\tests\text\Test_Regex.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\text\Test_String.c -o obj\win\x86_64\clang\clang-debug\tests\text\Test_String.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\tests\text\Test_Unicode.c -o obj\win\x86_64\clang\clang-debug\tests\text\Test_Unicode.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\BinaryHeap.c -o obj\win\x86_64\clang\clang-debug\src\containers\BinaryHeap.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Container.c -o obj\win\x86_64\clang\clang-debug\src\containers\Container.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Hashtable.c -o obj\win\x86_64\clang\clang-debug\src\containers\Hashtable.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Iterator.c -o obj\win\x86_64\clang\clang-debug\src\containers\Iterator.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\List.c -o obj\win\x86_64\clang\clang-debug\src\containers\List.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\containers\Vector.c -o obj\win\x86_64\clang\clang-debug\src\containers\Vector.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Process.c -o obj\win\x86_64\clang\clang-debug\src\os\Process.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Signal.c -o obj\win\x86_64\clang\clang-debug\src\os\Signal.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\System.c -o obj\win\x86_64\clang\clang-debug\src\os\System.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\os\Timer.c -o obj\win\x86_64\clang\clang-debug\src\os\Timer.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\Regex.c -o obj\win\x86_64\clang\clang-debug\src\text\Regex.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\String.c -o obj\win\x86_64\clang\clang-debug\src\text\String.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\Unicode.c -o obj\win\x86_64\clang\clang-debug\src\text\Unicode.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\text\UnicodeData.c -o obj\win\x86_64\clang\clang-debug\src\text\UnicodeData.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\threading\Thread.c -o obj\win\x86_64\clang\clang-debug\src\threading\Thread.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Array.c -o obj\win\x86_64\clang\clang-debug\src\Array.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Cstring.c -o obj\win\x86_64\clang\clang-debug\src\Cstring.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Error.c -o obj\win\x86_64\clang\clang-debug\src\Error.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\IO.c -o obj\win\x86_64\clang\clang-debug\src\IO.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Mem.c -o obj\win\x86_64\clang\clang-debug\src\Mem.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Slice.c -o obj\win\x86_64\clang\clang-debug\src\Slice.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\src\Std.c -o obj\win\x86_64\clang\clang-debug\src\Std.o 
clang   -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c src\os\WinEx.c -o obj\win\x86_64\clang\clang-debug\src\os\WinEx.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\bool.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\bool.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\char.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\char.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\int.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\int.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\null.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\null.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\ptr.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\ptr.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\result.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\result.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\size_t.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\size_t.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert\str.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert\str.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\assert.c -o obj\win\x86_64\clang\clang-debug\flut\src\assert.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\context.c -o obj\win\x86_64\clang\clang-debug\flut\src\context.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\expect.c -o obj\win\x86_64\clang\clang-debug\flut\src\expect.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\flut.c -o obj\win\x86_64\clang\clang-debug\flut\src\flut.o 
clang  -I.\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=x86_64-pc-win32 -ggdb -O0 -c .\flut\src\suite.c -o obj\win\x86_64\clang\clang-debug\flut\src\suite.o 
llvm-lib.exe /NOLOGO /OUT:build\win\x86_64\clang\clang-debug\libflut.lib  obj\win\x86_64\clang\clang-debug\src\containers\BinaryHeap.o obj\win\x86_64\clang\clang-debug\src\containers\Container.o obj\win\x86_64\clang\clang-debug\src\containers\Hashtable.o obj\win\x86_64\clang\clang-debug\src\containers\Iterator.o obj\win\x86_64\clang\clang-debug\src\containers\List.o obj\win\x86_64\clang\clang-debug\src\containers\Vector.o obj\win\x86_64\clang\clang-debug\src\os\Process.o obj\win\x86_64\clang\clang-debug\src\os\Signal.o obj\win\x86_64\clang\clang-debug\src\os\System.o obj\win\x86_64\clang\clang-debug\src\os\Timer.o obj\win\x86_64\clang\clang-debug\src\text\Regex.o obj\win\x86_64\clang\clang-debug\src\text\String.o obj\win\x86_64\clang\clang-debug\src\text\Unicode.o obj\win\x86_64\clang\clang-debug\src\text\UnicodeData.o obj\win\x86_64\clang\clang-debug\src\threading\Thread.o obj\win\x86_64\clang\clang-debug\src\Array.o obj\win\x86_64\clang\clang-debug\src\Cstring.o obj\win\x86_64\clang\clang-debug\src\Error.o obj\win\x86_64\clang\clang-debug\src\IO.o obj\win\x86_64\clang\clang-debug\src\Mem.o obj\win\x86_64\clang\clang-debug\src\Slice.o obj\win\x86_64\clang\clang-debug\src\Std.o obj\win\x86_64\clang\clang-debug\src\os\WinEx.o obj\win\x86_64\clang\clang-debug\flut\src\assert\bool.o obj\win\x86_64\clang\clang-debug\flut\src\assert\char.o obj\win\x86_64\clang\clang-debug\flut\src\assert\int.o obj\win\x86_64\clang\clang-debug\flut\src\assert\null.o obj\win\x86_64\clang\clang-debug\flut\src\assert\ptr.o obj\win\x86_64\clang\clang-debug\flut\src\assert\result.o obj\win\x86_64\clang\clang-debug\flut\src\assert\size_t.o obj\win\x86_64\clang\clang-debug\flut\src\assert\str.o obj\win\x86_64\clang\clang-debug\flut\src\assert.o obj\win\x86_64\clang\clang-debug\flut\src\context.o obj\win\x86_64\clang\clang-debug\flut\src\expect.o obj\win\x86_64\clang\clang-debug\flut\src\flut.o obj\win\x86_64\clang\clang-debug\flut\src\suite.o
link.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL /OUT:build\win\x86_64\clang\clang-debug\tests.exe   obj\win\x86_64\clang\clang-debug\tests\array.unit.o obj\win\x86_64\clang\clang-debug\tests\containers\Test_BinaryHeap.o obj\win\x86_64\clang\clang-debug\tests\containers\Test_Hashtable.o obj\win\x86_64\clang\clang-debug\tests\containers\Test_List.o obj\win\x86_64\clang\clang-debug\tests\containers\Test_Vector.o obj\win\x86_64\clang\clang-debug\tests\cstring.unit.o obj\win\x86_64\clang\clang-debug\tests\IO.o obj\win\x86_64\clang\clang-debug\tests\main.unit.o obj\win\x86_64\clang\clang-debug\tests\slice.unit.o obj\win\x86_64\clang\clang-debug\tests\std.unit.o obj\win\x86_64\clang\clang-debug\tests\text\Test_Regex.o obj\win\x86_64\clang\clang-debug\tests\text\Test_String.o obj\win\x86_64\clang\clang-debug\tests\text\Test_Unicode.o build\win\x86_64\clang\clang-debug\libflut.lib 
