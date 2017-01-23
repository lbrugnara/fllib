md .\obj\debug
md .\obj\debug\threading
md .\obj\debug\containers
md .\obj\debug\os
md .\obj\debug\text

clang -O3 -c .\src\threading\Thread.c  -o obj\debug\threading\Thread.o
clang -O3 -c .\src\Std.c  -o obj\debug\Std.o
clang -O3 -c .\src\Error.c  -o obj\debug\Error.o
clang -O3 -c .\src\Mem.c  -o obj\debug\Mem.o
clang -O3 -c .\src\Cstr.c  -o obj\debug\Cstr.o
clang -O3 -c .\src\Array.c  -o obj\debug\Array.o
clang -O3 -c .\src\File.c  -o obj\debug\File.o
clang -O3 -c .\src\containers\Iterator.c  -o obj\debug\containers\Iterator.o
clang -O3 -c .\src\containers\Vector.c  -o obj\debug\containers\Vector.o
clang -O3 -c .\src\containers\List.c  -o obj\debug\containers\List.o
clang -O3 -c .\src\containers\KeyValuePair.c  -o obj\debug\containers\KeyValuePair.o
clang -O3 -c .\src\containers\Dictionary.c  -o obj\debug\containers\Dictionary.o
clang -O3 -c .\src\text\Regex.c  -o obj\debug\text\Regex.o
clang -O3 -c .\src\os\System.c  -o obj\debug\os\System.o
clang -O3 -c .\src\os\Signal.c  -o obj\debug\os\Signal.o
clang -O3 -c .\src\os\WinEx.c -o obj\debug\os\WinEx.o

md .\build\debug

lib -NOLOGO -VERBOSE                        ^
    obj\debug\threading\Thread.o            ^
    obj\debug\Std.o                         ^
    obj\debug\Error.o                       ^
    obj\debug\Mem.o                         ^
    obj\debug\Cstr.o                        ^
    obj\debug\Array.o                       ^
    obj\debug\File.o                        ^
    obj\debug\containers\Iterator.o         ^
    obj\debug\containers\Vector.o           ^
    obj\debug\containers\List.o             ^
    obj\debug\containers\KeyValuePair.o     ^
    obj\debug\containers\Dictionary.o       ^
    obj\debug\text\Regex.o                  ^
    obj\debug\os\System.o                   ^
    obj\debug\os\Signal.o                   ^
    obj\debug\os\WinEx.o                    ^
    -OUT:build\debug\libfl.lib

:: Tests

md .\tests\obj\debug

clang -O3 -I.\include -I.\src -c tests\Main.c -o tests\obj\debug\Main.o
clang -O3 -I.\include -I.\src -c tests\Test.c -o tests\obj\debug\Test.o
clang -O3 -I.\include -I.\src -c tests\Std.c -o tests\obj\debug\Std.o
clang -O3 -I.\include -I.\src -c tests\File.c -o tests\obj\debug\File.o
clang -O3 -I.\include -I.\src -c tests\Cstr.c -o tests\obj\debug\Cstr.o

md .\tests\build\debug

clang -O3 -I.\include -I.\src -o tests\build\debug\tests.exe ^
    build\debug\libfl.lib   ^
    tests\obj\debug\Main.o  ^
    tests\obj\debug\Test.o  ^
    tests\obj\debug\Std.o   ^
    tests\obj\debug\File.o  ^
    tests\obj\debug\Cstr.o