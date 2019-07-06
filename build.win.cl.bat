set ARCH=%VSCMD_ARG_HOST_ARCH%

set CFLAGS=/Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG

set LIBSPATH=
set LIBS=build\debug\libfl.lib 

if "%~2%"=="sanitize" (
    set CFLAGS=%CFLAGS% -fsanitize=address,undefined
    if "%ARCH%"=="x64" (
        set LIBSPATH=%LIBSPATH% /LIBPATH:D:\dev\LLVM\x64\lib\clang\8.0.0\lib\windows
        set LIBS=%LIBS% clang_rt.ubsan_standalone-x86_64.lib clang_rt.asan-x86_64.lib clang_rt.builtins-x86_64.lib
    )
    if "%ARCH%"=="x86" (
        set LIBSPATH=%LIBSPATH% /LIBPATH:D:\dev\LLVM\x86\lib\clang\8.0.0\lib\windows
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

cl.exe %CFLAGS% /c .\src\threading\Thread.c  /Fo.\obj\debug\threading\Thread.o
cl.exe %CFLAGS% /c .\src\Std.c  /Fo.\obj\debug\Std.o
cl.exe %CFLAGS% /c .\src\Error.c  /Fo.\obj\debug\Error.o
cl.exe %CFLAGS% /c .\src\Mem.c  /Fo.\obj\debug\Mem.o
cl.exe %CFLAGS% /c .\src\text\resources\UnicodeData.c  /Fo.\obj\debug\text\resources\UnicodeData.o
cl.exe %CFLAGS% /c .\src\text\Unicode.c  /Fo.\obj\debug\text\Unicode.o
cl.exe %CFLAGS% /c .\src\text\String.c  /Fo.\obj\debug\text\String.o
cl.exe %CFLAGS% /c .\src\Cstring.c  /Fo.\obj\debug\Cstring.o
cl.exe %CFLAGS% /c .\src\Slice.c  /Fo.\obj\debug\Slice.o
cl.exe %CFLAGS% /c .\src\Array.c  /Fo.\obj\debug\Array.o
cl.exe %CFLAGS% /c .\src\IO.c  /Fo.\obj\debug\IO.o
cl.exe %CFLAGS% /c .\src\containers\Container.c  /Fo.\obj\debug\containers\Container.o
cl.exe %CFLAGS% /c .\src\containers\Iterator.c  /Fo.\obj\debug\containers\Iterator.o
cl.exe %CFLAGS% /c .\src\containers\Vector.c  /Fo.\obj\debug\containers\Vector.o
cl.exe %CFLAGS% /c .\src\containers\List.c  /Fo.\obj\debug\containers\List.o
cl.exe %CFLAGS% /c .\src\containers\Hashtable.c  /Fo.\obj\debug\containers\Hashtable.o
cl.exe %CFLAGS% /c .\src\text\Regex.c  /Fo.\obj\debug\text\Regex.o
cl.exe %CFLAGS% /c .\src\os\System.c  /Fo.\obj\debug\os\System.o
cl.exe %CFLAGS% /c .\src\os\Process.c  /Fo.\obj\debug\os\Process.o
cl.exe %CFLAGS% /c .\src\os\Timer.c  /Fo.\obj\debug\os\Timer.o
cl.exe %CFLAGS% /c .\src\os\Signal.c  /Fo.\obj\debug\os\Signal.o
cl.exe %CFLAGS% /c .\src\os\WinEx.c /Fo.\obj\debug\os\WinEx.o

md .\build\debug

lib -NOLOGO -VERBOSE                        ^
    obj\debug\threading\Thread.o            ^
    obj\debug\Std.o                         ^
    obj\debug\Slice.o                        ^
    obj\debug\Error.o                       ^
    obj\debug\Mem.o                         ^
    obj\debug\text\resources\UnicodeData.o  ^
    obj\debug\text\Unicode.o                ^
    obj\debug\text\String.o                 ^
    obj\debug\Cstring.o                     ^
    obj\debug\Array.o                       ^
    obj\debug\IO.o                          ^
    obj\debug\containers\Container.o        ^
    obj\debug\containers\Iterator.o         ^
    obj\debug\containers\Vector.o           ^
    obj\debug\containers\List.o             ^
    obj\debug\containers\Hashtable.o        ^
    obj\debug\text\Regex.o                  ^
    obj\debug\os\System.o                   ^
    obj\debug\os\Process.o                   ^
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

set TEST_CFLAGS=%CFLAGS% -I.\include -I.\src

cl.exe %TEST_CFLAGS% /c tests\Main.c /Fo.\tests\obj\debug\Main.o
cl.exe %TEST_CFLAGS% /c tests\Test.c /Fo.\tests\obj\debug\Test.o
cl.exe %TEST_CFLAGS% /c tests\Std.c /Fo.\tests\obj\debug\Std.o
cl.exe %TEST_CFLAGS% /c tests\Test_Slice.c /Fo.\tests\obj\debug\Test_Slice.o
cl.exe %TEST_CFLAGS% /c tests\IO.c /Fo.\tests\obj\debug\IO.o
cl.exe %TEST_CFLAGS% /c tests\Test_Cstring.c /Fo.\tests\obj\debug\Test_Cstring.o
cl.exe %TEST_CFLAGS% /c tests\text\Test_Unicode.c /Fo.\tests\obj\debug\text\Test_Unicode.o
cl.exe %TEST_CFLAGS% /c tests\text\Test_String.c /Fo.\tests\obj\debug\text\Test_String.o
cl.exe %TEST_CFLAGS% /c tests\text\Test_Regex.c /Fo.\tests\obj\debug\text\Test_Regex.o
cl.exe %TEST_CFLAGS% /c tests\containers\Test_Vector.c /Fo.\tests\obj\debug\containers\Test_Vector.o
cl.exe %TEST_CFLAGS% /c tests\containers\Test_Hashtable.c /Fo.\tests\obj\debug\containers\Test_Hashtable.o
cl.exe %TEST_CFLAGS% /c tests\containers\Test_List.c /Fo.\tests\obj\debug\containers\Test_List.o

:: Link
:LINK

rd /s /q "tests\build\"

md .\tests\build\debug

link.exe ^
    /OUT:tests\build\debug\tests.exe ^
    /DEFAULTLIB:libcmt ^
    /NOLOGO ^
    /DEBUG:FULL ^
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
