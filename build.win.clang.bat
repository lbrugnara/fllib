rd /s /q "obj\"
rd /s /q "build\"
rd /s /q "tests\obj\"
rd /s /q "tests\build\"

md .\obj\debug
md .\obj\debug\threading
md .\obj\debug\containers
md .\obj\debug\os
md .\obj\debug\text\resources

clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\threading\Thread.c  -o .\obj\debug\threading\Thread.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Std.c  -o .\obj\debug\Std.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Error.c  -o .\obj\debug\Error.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Mem.c  -o .\obj\debug\Mem.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\text\resources\UnicodeData.c  -o .\obj\debug\text\resources\UnicodeData.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\text\Unicode.c  -o .\obj\debug\text\Unicode.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\text\String.c  -o .\obj\debug\text\String.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Cstring.c  -o .\obj\debug\Cstring.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Span.c  -o .\obj\debug\Span.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\Array.c  -o .\obj\debug\Array.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\IO.c  -o .\obj\debug\IO.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\Iterator.c  -o .\obj\debug\containers\Iterator.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\Vector.c  -o .\obj\debug\containers\Vector.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\List.c  -o .\obj\debug\containers\List.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\KeyValuePair.c  -o .\obj\debug\containers\KeyValuePair.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\Dictionary.c  -o .\obj\debug\containers\Dictionary.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\containers\Hashtable.c  -o .\obj\debug\containers\Hashtable.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\text\Regex.c  -o .\obj\debug\text\Regex.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\os\System.c  -o .\obj\debug\os\System.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\os\Timer.c  -o .\obj\debug\os\Timer.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\os\Signal.c  -o .\obj\debug\os\Signal.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c .\src\os\WinEx.c -o .\obj\debug\os\WinEx.o

md .\build\debug

lib -NOLOGO -VERBOSE                        ^
    obj\debug\threading\Thread.o            ^
    obj\debug\Std.o                         ^
    obj\debug\Error.o                       ^
    obj\debug\Mem.o                         ^
    obj\debug\text\resources\UnicodeData.o  ^
    obj\debug\text\Unicode.o                ^
    obj\debug\text\String.o                 ^
    obj\debug\Cstring.o                     ^
    obj\debug\Span.o                        ^
    obj\debug\Array.o                       ^
    obj\debug\IO.o                          ^
    obj\debug\containers\Iterator.o         ^
    obj\debug\containers\Vector.o           ^
    obj\debug\containers\List.o             ^
    obj\debug\containers\KeyValuePair.o     ^
    obj\debug\containers\Dictionary.o       ^
    obj\debug\containers\Hashtable.o        ^
    obj\debug\text\Regex.o                  ^
    obj\debug\os\System.o                   ^
    obj\debug\os\Timer.o                    ^
    obj\debug\os\Signal.o                   ^
    obj\debug\os\WinEx.o                    ^
    -OUT:build\debug\libfl.lib

:: Tests

md .\tests\obj\debug
md .\tests\obj\debug\threading
md .\tests\obj\debug\text
md .\tests\obj\debug\containers

clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\Main.c -o .\tests\obj\debug\Main.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\Test.c -o .\tests\obj\debug\Test.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\Std.c -o .\tests\obj\debug\Std.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\Test_Span.c -o .\tests\obj\debug\Test_Span.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\IO.c -o .\tests\obj\debug\IO.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\Test_Cstring.c -o .\tests\obj\debug\Test_Cstring.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\text\Test_Unicode.c -o .\tests\obj\debug\text\Test_Unicode.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\text\Test_String.c -o .\tests\obj\debug\text\Test_String.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\text\Test_Regex.c -o .\tests\obj\debug\text\Test_Regex.o
clang.exe -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src tests\containers\Test_Hashtable.c -o .\tests\obj\debug\containers\Test_Hashtable.o

md .\tests\build\debug

:: clang.exe -v -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -c -I.\include -I.\src -o .\tests\build\debug\tests.exe ^
::     build\debug\libfl.lib   ^
::     tests\obj\debug\Main.o  ^
::     tests\obj\debug\Test.o  ^
::     tests\obj\debug\Std.o   ^
::     tests\obj\debug\IO.o  ^
::     tests\obj\debug\text\Test_Unicode.o  ^
::     tests\obj\debug\text\Test_String.o  ^
::     tests\obj\debug\Test_Cstring.o

link.exe /out:tests\\build\\debug\\tests.exe ^
    /defaultlib:libcmt ^
    /nologo ^
    /DEBUG:FULL ^
    build\\debug\\libfl.lib ^
    tests\\obj\\debug\\Main.o ^
    tests\\obj\\debug\\Test.o ^
    tests\\obj\\debug\\Std.o ^
    tests\\obj\\debug\\Test_Span.o ^
    tests\\obj\\debug\\IO.o ^
    tests\\obj\\debug\\Test_Cstring.o ^
    tests\\obj\\debug\\text\\Test_Unicode.o ^
    tests\\obj\\debug\\text\\Test_String.o ^
    tests\\obj\\debug\\text\\Test_regex.o ^
    tests\\obj\\debug\\containers\\Test_Hashtable.o
