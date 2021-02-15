mkdir     obj\win\x86_64\msvc\msvc-debug\src     obj\win\x86_64\msvc\msvc-debug\src\containers     obj\win\x86_64\msvc\msvc-debug\src\os     obj\win\x86_64\msvc\msvc-debug\src\text     obj\win\x86_64\msvc\msvc-debug\src\threading 
mkdir     obj\win\x86_64\msvc\msvc-debug\flut\src     obj\win\x86_64\msvc\msvc-debug\flut\src\assert 
mkdir     obj\win\x86_64\msvc\msvc-debug\tests\     obj\win\x86_64\msvc\msvc-debug\tests\containers     obj\win\x86_64\msvc\msvc-debug\tests\text  
mkdir build\win\x86_64\msvc\msvc-debug 
if not exist .\include (mkdir .\include)
if not exist .\include\fllib (mkdir .\include\fllib)
if not exist .\include\flut (mkdir .\include\flut)
xcopy /F /y /d /s .\src\*.h .\include\fllib
xcopy /F /y /d /s .\flut\src\*.h .\include\flut
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\BinaryHeap.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Container.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Container.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Hashtable.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Hashtable.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Iterator.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Iterator.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\List.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\List.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Vector.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Vector.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Process.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Process.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Signal.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Signal.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\System.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\System.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Timer.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Timer.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\Regex.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\Regex.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\String.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\String.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\Unicode.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\Unicode.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\UnicodeData.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\UnicodeData.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\threading\Thread.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\threading\Thread.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Array.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Array.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Cstring.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Cstring.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Error.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Error.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\IO.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\IO.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Mem.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Mem.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Slice.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Slice.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Std.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Std.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c src\os\WinEx.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\WinEx.o 
lib.exe /NOLOGO /OUT:build\win\x86_64\msvc\msvc-debug\libfl.lib  obj\win\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o obj\win\x86_64\msvc\msvc-debug\src\containers\Container.o obj\win\x86_64\msvc\msvc-debug\src\containers\Hashtable.o obj\win\x86_64\msvc\msvc-debug\src\containers\Iterator.o obj\win\x86_64\msvc\msvc-debug\src\containers\List.o obj\win\x86_64\msvc\msvc-debug\src\containers\Vector.o obj\win\x86_64\msvc\msvc-debug\src\os\Process.o obj\win\x86_64\msvc\msvc-debug\src\os\Signal.o obj\win\x86_64\msvc\msvc-debug\src\os\System.o obj\win\x86_64\msvc\msvc-debug\src\os\Timer.o obj\win\x86_64\msvc\msvc-debug\src\text\Regex.o obj\win\x86_64\msvc\msvc-debug\src\text\String.o obj\win\x86_64\msvc\msvc-debug\src\text\Unicode.o obj\win\x86_64\msvc\msvc-debug\src\text\UnicodeData.o obj\win\x86_64\msvc\msvc-debug\src\threading\Thread.o obj\win\x86_64\msvc\msvc-debug\src\Array.o obj\win\x86_64\msvc\msvc-debug\src\Cstring.o obj\win\x86_64\msvc\msvc-debug\src\Error.o obj\win\x86_64\msvc\msvc-debug\src\IO.o obj\win\x86_64\msvc\msvc-debug\src\Mem.o obj\win\x86_64\msvc\msvc-debug\src\Slice.o obj\win\x86_64\msvc\msvc-debug\src\Std.o obj\win\x86_64\msvc\msvc-debug\src\os\WinEx.o
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\array.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\array.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\containers\binary_heap.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\containers\binary_heap.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\containers\hashtable.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\containers\hashtable.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\containers\list.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\containers\list.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\containers\vector.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\containers\vector.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\cstring.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\cstring.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\io.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\io.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\main.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\main.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\slice.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\slice.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\std.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\std.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\text\regex.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\text\regex.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\text\string.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\text\string.unit.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\text\unicode.unit.c /Fo.\obj\win\x86_64\msvc\msvc-debug\tests\text\unicode.unit.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\BinaryHeap.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Container.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Container.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Hashtable.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Hashtable.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Iterator.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Iterator.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\List.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\List.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\containers\Vector.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\containers\Vector.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Process.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Process.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Signal.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Signal.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\System.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\System.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\os\Timer.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\Timer.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\Regex.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\Regex.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\String.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\String.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\Unicode.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\Unicode.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\text\UnicodeData.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\text\UnicodeData.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\threading\Thread.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\threading\Thread.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Array.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Array.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Cstring.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Cstring.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Error.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Error.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\IO.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\IO.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Mem.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Mem.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Slice.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Slice.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c .\src\Std.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\Std.o 
cl.exe   /W1 /Od /Z7 /DEBUG:FULL /c src\os\WinEx.c /Fo.\obj\win\x86_64\msvc\msvc-debug\src\os\WinEx.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\assertion.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\assertion.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\bool.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\bool.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\char.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\char.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\double.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\double.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\int.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\int.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\null.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\null.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\ptr.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\ptr.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\result.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\result.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\size_t.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\size_t.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\assert\str.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\assert\str.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\context.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\context.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\expect.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\expect.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\flut.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\flut.o 
cl.exe  /I.\include  /W1 /Od /Z7 /DEBUG:FULL /c .\flut\src\suite.c /Fo.\obj\win\x86_64\msvc\msvc-debug\flut\src\suite.o 
lib.exe /NOLOGO /OUT:build\win\x86_64\msvc\msvc-debug\libflut.lib  obj\win\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o obj\win\x86_64\msvc\msvc-debug\src\containers\Container.o obj\win\x86_64\msvc\msvc-debug\src\containers\Hashtable.o obj\win\x86_64\msvc\msvc-debug\src\containers\Iterator.o obj\win\x86_64\msvc\msvc-debug\src\containers\List.o obj\win\x86_64\msvc\msvc-debug\src\containers\Vector.o obj\win\x86_64\msvc\msvc-debug\src\os\Process.o obj\win\x86_64\msvc\msvc-debug\src\os\Signal.o obj\win\x86_64\msvc\msvc-debug\src\os\System.o obj\win\x86_64\msvc\msvc-debug\src\os\Timer.o obj\win\x86_64\msvc\msvc-debug\src\text\Regex.o obj\win\x86_64\msvc\msvc-debug\src\text\String.o obj\win\x86_64\msvc\msvc-debug\src\text\Unicode.o obj\win\x86_64\msvc\msvc-debug\src\text\UnicodeData.o obj\win\x86_64\msvc\msvc-debug\src\threading\Thread.o obj\win\x86_64\msvc\msvc-debug\src\Array.o obj\win\x86_64\msvc\msvc-debug\src\Cstring.o obj\win\x86_64\msvc\msvc-debug\src\Error.o obj\win\x86_64\msvc\msvc-debug\src\IO.o obj\win\x86_64\msvc\msvc-debug\src\Mem.o obj\win\x86_64\msvc\msvc-debug\src\Slice.o obj\win\x86_64\msvc\msvc-debug\src\Std.o obj\win\x86_64\msvc\msvc-debug\src\os\WinEx.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\assertion.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\bool.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\char.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\double.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\int.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\null.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\ptr.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\result.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\size_t.o obj\win\x86_64\msvc\msvc-debug\flut\src\assert\str.o obj\win\x86_64\msvc\msvc-debug\flut\src\context.o obj\win\x86_64\msvc\msvc-debug\flut\src\expect.o obj\win\x86_64\msvc\msvc-debug\flut\src\flut.o obj\win\x86_64\msvc\msvc-debug\flut\src\suite.o
link.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /MACHINE:x64 /DEBUG:FULL /OUT:build\win\x86_64\msvc\msvc-debug\tests.exe   obj\win\x86_64\msvc\msvc-debug\tests\array.unit.o obj\win\x86_64\msvc\msvc-debug\tests\containers\binary_heap.unit.o obj\win\x86_64\msvc\msvc-debug\tests\containers\hashtable.unit.o obj\win\x86_64\msvc\msvc-debug\tests\containers\list.unit.o obj\win\x86_64\msvc\msvc-debug\tests\containers\vector.unit.o obj\win\x86_64\msvc\msvc-debug\tests\cstring.unit.o obj\win\x86_64\msvc\msvc-debug\tests\io.unit.o obj\win\x86_64\msvc\msvc-debug\tests\main.unit.o obj\win\x86_64\msvc\msvc-debug\tests\slice.unit.o obj\win\x86_64\msvc\msvc-debug\tests\std.unit.o obj\win\x86_64\msvc\msvc-debug\tests\text\regex.unit.o obj\win\x86_64\msvc\msvc-debug\tests\text\string.unit.o obj\win\x86_64\msvc\msvc-debug\tests\text\unicode.unit.o build\win\x86_64\msvc\msvc-debug\libflut.lib 
