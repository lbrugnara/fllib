rd /s /q "obj\"
rd /s /q "build\"
rd /s /q "tests\obj\"
rd /s /q "tests\build\"

md .\obj\debug
md .\obj\debug\threading
md .\obj\debug\containers
md .\obj\debug\os
md .\obj\debug\text\resources

cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\threading\Thread.c  /Fo.\obj\debug\threading\Thread.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Std.c  /Fo.\obj\debug\Std.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Error.c  /Fo.\obj\debug\Error.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Mem.c  /Fo.\obj\debug\Mem.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\text\resources\UnicodeData.c  /Fo.\obj\debug\text\resources\UnicodeData.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\text\Unicode.c  /Fo.\obj\debug\text\Unicode.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\text\String.c  /Fo.\obj\debug\text\String.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Cstring.c  /Fo.\obj\debug\Cstring.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Span.c  /Fo.\obj\debug\Span.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\Array.c  /Fo.\obj\debug\Array.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\IO.c  /Fo.\obj\debug\IO.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\containers\Container.c  /Fo.\obj\debug\containers\Container.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\containers\Iterator.c  /Fo.\obj\debug\containers\Iterator.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\containers\Vector.c  /Fo.\obj\debug\containers\Vector.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\containers\List.c  /Fo.\obj\debug\containers\List.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\containers\Hashtable.c  /Fo.\obj\debug\containers\Hashtable.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\text\Regex.c  /Fo.\obj\debug\text\Regex.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\os\System.c  /Fo.\obj\debug\os\System.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\os\Timer.c  /Fo.\obj\debug\os\Timer.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\os\Signal.c  /Fo.\obj\debug\os\Signal.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c .\src\os\WinEx.c /Fo.\obj\debug\os\WinEx.o

md .\build\debug

lib -NOLOGO -VERBOSE                        ^
    obj\debug\threading\Thread.o            ^
    obj\debug\Std.o                         ^
    obj\debug\Span.o                        ^
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
    obj\debug\os\Timer.o                    ^
    obj\debug\os\Signal.o                   ^
    obj\debug\os\WinEx.o                    ^
    -OUT:build\debug\libfl.lib

:: Tests

md .\tests\obj\debug
md .\tests\obj\debug\threading
md .\tests\obj\debug\text
md .\tests\obj\debug\containers

cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\Main.c /Fo.\tests\obj\debug\Main.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\Test.c /Fo.\tests\obj\debug\Test.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\Std.c /Fo.\tests\obj\debug\Std.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\Test_Span.c /Fo.\tests\obj\debug\Test_Span.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\IO.c /Fo.\tests\obj\debug\IO.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\Test_Cstring.c /Fo.\tests\obj\debug\Test_Cstring.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\text\Test_Unicode.c /Fo.\tests\obj\debug\text\Test_Unicode.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\text\Test_String.c /Fo.\tests\obj\debug\text\Test_String.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\text\Test_Regex.c /Fo.\tests\obj\debug\text\Test_Regex.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\containers\Test_Hashtable.c /Fo.\tests\obj\debug\containers\Test_Hashtable.o
cl.exe /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src tests\containers\Test_List.c /Fo.\tests\obj\debug\containers\Test_List.o

md .\tests\build\debug

:: cl.exe -v /Zi /DEBUG:FULL /W1 /DFL_UNICODE_DB /DFL_DEBUG /c -I.\include -I.\src /Fo.\tests\build\debug\tests.exe ^
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
    tests\\obj\\debug\\containers\\Test_Hashtable.o ^
    tests\\obj\\debug\\containers\\Test_List.o
