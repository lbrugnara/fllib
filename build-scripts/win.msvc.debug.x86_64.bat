mkdir obj\\x86_64 
if not exist .\include (mkdir .\include)
if not exist .\include\fllib (mkdir .\include\fllib)
if not exist .\include\flut (mkdir .\include\flut)
xcopy /y /d /s .\src\*.h .\include\fllib
xcopy /y /d /s .\flut\src\*.h .\include\flut
cl.exe   /W1 /c .\src\containers\BinaryHeap.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Container.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Container.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Hashtable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Hashtable.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Iterator.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Iterator.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\List.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\List.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Vector.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Vector.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Process.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Process.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Signal.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Signal.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\System.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\System.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Timer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Timer.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\Regex.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Regex.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\String.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\String.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\Unicode.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Unicode.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\UnicodeData.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\UnicodeData.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\threading\Thread.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\threading\Thread.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Array.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Array.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Cstring.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Cstring.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Error.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Error.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\IO.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\IO.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Mem.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Mem.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Slice.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Slice.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Std.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Std.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c src\os\WinEx.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\WinEx.o /Od /Zi /DEBUG:FULL 
lib.exe /NOLOGO /OUT:build\win-cmd\x86_64\msvc\msvc-debug\libfl.lib  obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Container.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Hashtable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Iterator.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\List.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Vector.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Process.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Signal.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\System.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Timer.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Regex.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\String.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Unicode.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\UnicodeData.o obj\win-cmd\x86_64\msvc\msvc-debug\src\threading\Thread.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Array.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Cstring.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Error.o obj\win-cmd\x86_64\msvc\msvc-debug\src\IO.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Mem.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Slice.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Std.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\WinEx.o
cl.exe  /I.\include  /W1 /c .\tests\containers\Test_BinaryHeap.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_BinaryHeap.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\containers\Test_Hashtable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_Hashtable.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\containers\Test_List.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_List.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\containers\Test_Vector.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_Vector.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\IO.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\IO.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\Main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Main.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\Std.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Std.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\Test_Array.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Array.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\Test_Cstring.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Cstring.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\Test_Slice.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Slice.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\text\Test_Regex.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_Regex.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\text\Test_String.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_String.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\tests\text\Test_Unicode.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_Unicode.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\BinaryHeap.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Container.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Container.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Hashtable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Hashtable.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Iterator.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Iterator.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\List.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\List.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\containers\Vector.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Vector.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Process.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Process.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Signal.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Signal.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\System.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\System.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\os\Timer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Timer.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\Regex.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Regex.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\String.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\String.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\Unicode.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Unicode.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\text\UnicodeData.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\text\UnicodeData.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\threading\Thread.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\threading\Thread.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Array.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Array.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Cstring.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Cstring.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Error.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Error.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\IO.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\IO.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Mem.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Mem.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Slice.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Slice.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c .\src\Std.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\Std.o /Od /Zi /DEBUG:FULL 
cl.exe   /W1 /c src\os\WinEx.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\os\WinEx.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert\bool.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\bool.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert\null.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\null.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert\result.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\result.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert\size_t.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\size_t.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert\str.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\str.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\assert.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\context.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\context.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\expect.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\expect.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\flut.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\flut.o /Od /Zi /DEBUG:FULL 
cl.exe  /I.\include  /W1 /c .\flut\src\suite.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\suite.o /Od /Zi /DEBUG:FULL 
lib.exe /NOLOGO /OUT:build\win-cmd\x86_64\msvc\msvc-debug\libflut.lib  obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\BinaryHeap.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Container.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Hashtable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Iterator.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\List.o obj\win-cmd\x86_64\msvc\msvc-debug\src\containers\Vector.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Process.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Signal.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\System.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\Timer.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Regex.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\String.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\Unicode.o obj\win-cmd\x86_64\msvc\msvc-debug\src\text\UnicodeData.o obj\win-cmd\x86_64\msvc\msvc-debug\src\threading\Thread.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Array.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Cstring.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Error.o obj\win-cmd\x86_64\msvc\msvc-debug\src\IO.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Mem.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Slice.o obj\win-cmd\x86_64\msvc\msvc-debug\src\Std.o obj\win-cmd\x86_64\msvc\msvc-debug\src\os\WinEx.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\bool.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\null.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\result.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\size_t.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert\str.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\assert.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\context.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\expect.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\flut.o obj\win-cmd\x86_64\msvc\msvc-debug\flut\src\suite.o
link.exe /OUT:build\win-cmd\x86_64\msvc\msvc-debug\tests.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL   obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_BinaryHeap.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_Hashtable.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_List.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\containers\Test_Vector.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\IO.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Main.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Std.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Array.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Cstring.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test_Slice.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_Regex.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_String.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\text\Test_Unicode.o build\win-cmd\x86_64\msvc\msvc-debug\libflut.lib 
