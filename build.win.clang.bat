set ARCH=%VSCMD_ARG_HOST_ARCH%

set CFLAGS= -std=c99 ^
            -Wall ^
            -Wextra ^
            -pedantic ^
            -Wmissing-field-initializers ^
            -Wno-unused-parameter ^
            -Wno-unused-variable ^
            -Wno-unused-function ^
            -Wno-missing-braces ^
            -fstrict-aliasing ^
            -finput-charset=UTF-8 ^
            -fexec-charset=UTF-8 ^
            -ggdb ^
            -DFL_UNICODE_DB ^
            -D_FORTIFY_SOURCE=2 ^
            -DFL_DEBUG

set LIBSPATH=
set LIBS=build\debug\libfl.lib 

if "%~2%"=="sanitize" (
    set CFLAGS=%CFLAGS% -fsanitize=address,undefined
    if "%ARCH%"=="x64" (
        set LIBSPATH=%LIBSPATH% -LIBPATH:D:\dev\LLVM\x64\lib\clang\8.0.0\lib\windows
        set LIBS=%LIBS% clang_rt.ubsan_standalone-x86_64.lib clang_rt.asan-x86_64.lib clang_rt.builtins-x86_64.lib
    )
    if "%ARCH%"=="x86" (
        set LIBSPATH=%LIBSPATH% -LIBPATH:D:\dev\LLVM\x86\lib\clang\8.0.0\lib\windows
        set LIBS=%LIBS% clang_rt.ubsan_standalone-i386.lib clang_rt.asan-i386.lib clang_rt.builtins-i386.lib
    )
)

if "%~1%"=="fl" (
    GOTO :FLLIB
)

if "%~1%"=="tests" (
    GOTO :TESTS
)

if "%~1%"=="link" (
    GOTO :LINK
)

if "%~1%"=="clean" (
    GOTO :CLEAN
)

:: fllib
:FLLIB

rd /s /q "obj\"
rd /s /q "build\"
md .\obj\debug
md .\obj\debug\threading
md .\obj\debug\containers
md .\obj\debug\os
md .\obj\debug\text\resources

clang.exe %CFLAGS% -c .\src\threading\Thread.c  -o .\obj\debug\threading\Thread.o
clang.exe %CFLAGS% -c .\src\Std.c  -o .\obj\debug\Std.o
clang.exe %CFLAGS% -c .\src\Error.c  -o .\obj\debug\Error.o
clang.exe %CFLAGS% -c .\src\Mem.c  -o .\obj\debug\Mem.o
clang.exe %CFLAGS% -c .\src\text\resources\UnicodeData.c  -o .\obj\debug\text\resources\UnicodeData.o
clang.exe %CFLAGS% -c .\src\text\Unicode.c  -o .\obj\debug\text\Unicode.o
clang.exe %CFLAGS% -c .\src\text\String.c  -o .\obj\debug\text\String.o
clang.exe %CFLAGS% -c .\src\Cstring.c  -o .\obj\debug\Cstring.o
clang.exe %CFLAGS% -c .\src\Slice.c  -o .\obj\debug\Slice.o
clang.exe %CFLAGS% -c .\src\Array.c  -o .\obj\debug\Array.o
clang.exe %CFLAGS% -c .\src\IO.c  -o .\obj\debug\IO.o
clang.exe %CFLAGS% -c .\src\containers\Container.c  -o .\obj\debug\containers\Container.o
clang.exe %CFLAGS% -c .\src\containers\Iterator.c  -o .\obj\debug\containers\Iterator.o
clang.exe %CFLAGS% -c .\src\containers\Vector.c  -o .\obj\debug\containers\Vector.o
clang.exe %CFLAGS% -c .\src\containers\List.c  -o .\obj\debug\containers\List.o
clang.exe %CFLAGS% -c .\src\containers\Hashtable.c  -o .\obj\debug\containers\Hashtable.o
clang.exe %CFLAGS% -c .\src\text\Regex.c  -o .\obj\debug\text\Regex.o
clang.exe %CFLAGS% -c .\src\os\System.c  -o .\obj\debug\os\System.o
clang.exe %CFLAGS% -c .\src\os\Timer.c  -o .\obj\debug\os\Timer.o
clang.exe %CFLAGS% -c .\src\os\Signal.c  -o .\obj\debug\os\Signal.o
clang.exe %CFLAGS% -c .\src\os\WinEx.c -o .\obj\debug\os\WinEx.o

md .\build\debug

llvm-lib -NOLOGO -VERBOSE                        ^
    obj\debug\threading\Thread.o            ^
    obj\debug\Std.o                         ^
    obj\debug\Error.o                       ^
    obj\debug\Mem.o                         ^
    obj\debug\text\resources\UnicodeData.o  ^
    obj\debug\text\Unicode.o                ^
    obj\debug\text\String.o                 ^
    obj\debug\Cstring.o                     ^
    obj\debug\Slice.o                        ^
    obj\debug\Array.o                       ^
    obj\debug\IO.o                          ^
    obj\debug\containers\Container.o        ^
    obj\debug\containers\Iterator.o         ^
    obj\debug\containers\Vector.o           ^
    obj\debug\containers\List.o             ^
    obj\debug\containers\Hashtable.o        ^
    obj\debug\text\Regex.o                  ^
    obj\debug\os\System.o                   ^
    obj\debug\os\Timer.o                    ^
    obj\debug\os\Signal.o                   ^
    obj\debug\os\WinEx.o                    ^
    -OUT:build\debug\libfl.lib

:: Tests
:TESTS

rd /s /q "tests\obj\"

md .\tests\obj\debug
md .\tests\obj\debug\threading
md .\tests\obj\debug\text
md .\tests\obj\debug\containers

set TEST_CFLAGS=%CFLAGS% -I.\src -I.\include

clang.exe %TEST_CFLAGS% -c tests\Main.c -o .\tests\obj\debug\Main.o
clang.exe %TEST_CFLAGS% -c tests\Test.c -o .\tests\obj\debug\Test.o
clang.exe %TEST_CFLAGS% -c tests\Std.c -o .\tests\obj\debug\Std.o
clang.exe %TEST_CFLAGS% -c tests\Test_Slice.c -o .\tests\obj\debug\Test_Slice.o
clang.exe %TEST_CFLAGS% -c tests\IO.c -o .\tests\obj\debug\IO.o
clang.exe %TEST_CFLAGS% -c tests\Test_Cstring.c -o .\tests\obj\debug\Test_Cstring.o
clang.exe %TEST_CFLAGS% -c tests\text\Test_Unicode.c -o .\tests\obj\debug\text\Test_Unicode.o
clang.exe %TEST_CFLAGS% -c tests\text\Test_String.c -o .\tests\obj\debug\text\Test_String.o
clang.exe %TEST_CFLAGS% -c tests\text\Test_Regex.c -o .\tests\obj\debug\text\Test_Regex.o
clang.exe %TEST_CFLAGS% -c tests\containers\Test_Vector.c -o .\tests\obj\debug\containers\Test_Vector.o
clang.exe %TEST_CFLAGS% -c tests\containers\Test_Hashtable.c -o .\tests\obj\debug\containers\Test_Hashtable.o
clang.exe %TEST_CFLAGS% -c tests\containers\Test_List.c -o .\tests\obj\debug\containers\Test_List.o

:: Link
:LINK

rd /s /q "tests\build\"

md .\tests\build\debug

lld-link.exe ^
    -OUT:tests\build\debug\tests.exe ^
    -DEFAULTLIB:libcmt ^
    -NOLOGO ^
    -DEBUG:FULL ^
    %LIBSPATH% ^
    %LIBS% ^
    tests\obj\debug\Main.o ^
    tests\obj\debug\Test.o ^
    tests\obj\debug\Std.o ^
    tests\obj\debug\Test_Slice.o ^
    tests\obj\debug\IO.o ^
    tests\obj\debug\Test_Cstring.o ^
    tests\obj\debug\text\Test_Unicode.o ^
    tests\obj\debug\text\Test_String.o ^
    tests\obj\debug\text\Test_Regex.o ^
    tests\obj\debug\containers\Test_Hashtable.o ^
    tests\\obj\\debug\\containers\\Test_Vector.o ^
    tests\obj\debug\containers\Test_List.o

GOTO :EXIT

:CLEAN

rd /s /q "obj\"
rd /s /q "build\"
rd /s /q "tests\obj\"
rd /s /q "tests\build\"

:EXIT
