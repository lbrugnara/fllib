# fllib

The goal of this project is to experiment and learn through the implementation of common algorithms, data structures, and libraries written in portable C(99) aiming to get a unified API and predictable behavior between multiple platforms.

## Supported platforms

- **Windows**: MSVC, WSL, Clang, MinGW 32/64, Cygwin
- **Ubuntu**: GCC, Clang
- **Raspbian**: GCC
- **GalliumOS** *(Braswell)*: GCC, Clang

Architectures on these platforms: x86, x64, and ARM

## Modules

The following are the set of modules that are "usable", this means the functions defined in the module are tested and implemented across the different platforms, but the module may not be fully implemented.

#### Standard

These are common C data types and algorithms with an abstraction layer over them

- `Cstring`: Provides a set of functions to operate on `char*`
- `Array`: Dynamic array implementation that uses a header `struct` to track the number of elements and the data type size
- `Span`: An abstraction over a contiguous sequence of `const` bytes

#### Containers

Set of containers implemented using opaque pointers. All the containers (and other modules like `Span`) support `Iterator` through function pointers

- `Hashtable`: A generic implementation that uses function pointers to hash, allocate, and deallocate keys and values
- `List`: A doubly linked list
- `Vector`: Similar to the array module but using an opaque pointer

#### Text

Set of functions to work on sequences of bytes representing text

- `Regex`: A regex engine using NFAs (based on [Russ Cox's articles on regular expressions](https://swtch.com/~rsc/regexp))
- `Unicode`: A work-in-progress implementation of the Unicode Standard version 9.0 (Normalization and Case-Folding are the big *TODOs* of this module)

#### OS

Modules that work with different OS primitives

- `Thread`: Provides an abstraction layer over `pthread` and  `Win32` to work with threads, locks, and sync methods
- `Signal`: Basic and cross-platform signal handling
- `Timer`: High-resolution timers
- `IO`: Set of functions to work with files and directories

## Compiling

Below are the dependencies and steps to compile the library and build the tests on every platform. The build process is very "handcrafted" (especially for MSVC and Clang on Windows), but the make command accepts a couple of useful flags:

- `CC`: Compiler (default is GCC)
- `AR`: Archive utility
- `CFLAGS`: Compilation flags (check default flags in Makefile)
- `TARGET`: `debug` or `release`. By default, `debug`
- `LINKAGE`: `static` or `shared`. By default, `static`

For all the platforms, using the default flags, the compiled library should be placed in `build/debug` (`libfl.a` or `libfl.dll`) and the tests in `tests/build/debug` (`tests.exe`)

### Linux

#### Dependencies
- make
- GCC or Clang

#### Build 
```
make tests CC=[gcc|clang]
```

### Windows

#### Dependencies
- MSVC or Clang
- MinGW, Cygwin, WSL

#### Build
To compile on Windows using MSVC or Clang you can run one the following `.bat` files in the [x86 or x64 Native Tools Command Prompt](https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=vs-2017). If you want to compile it in an existing command window, you will need to load the environment variables for the particular architecture. For more information check this Microsoft page: [Build C/C++ code on the command line](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2017).

MSVC:
```
build.win.cl.bat
```
Clang:
```
build.win.clang.bat
```

If you want to use any other alternative under Windows, like MinGW, Cygwin or WSL, it should be as easy as running the following on each specific env:
```
make tests CC=[gcc|clang]
```

## Frequently Asked Questions

#### What motivated you to create this library?

I am a curious and self-taught programmer that loves to "destroy" the black-box to know how all the abstractions day after day I work upon work to better understand the benefits, the limitations, and to get an accurate and expected outcome from them. 

#### Why C?

For this project, I chose C because of 2 reasons: it is a simple yet powerful language that runs almost anywhere, and I want to improve my skills on memory management, pointers, portability, efficiency, and security while programming on it. Being that my day-to-day work is not in C, I think this may be the best way to learn it, and, on the other side, all the knowledge I gain with C is tremendously helpful to understand how other languages work under the hood.

#### Can I contribute to the project or submit suggestions?

Yes, sure. Being it is a project to experiment and learn, I encourage you to fork the project and contribute to it, or participate through the Issues. 

#### Is there documentation about the modules?

On every module, I try to document the functions and data structures, but the truth is, some modules might be outdated or just lacking documentation.

#### Are there usage examples?

Yes, check the `tests` folder and navigate through the different test suites. There you will find usages of the different modules functions.

#### License?

MIT
