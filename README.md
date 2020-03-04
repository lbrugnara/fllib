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
- `Slice`: An abstraction over a contiguous sequence of `const` bytes

#### Containers

Set of containers implemented using opaque pointers. All the containers (and other modules like `Slice`) support `Iterator` through function pointers

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

To compile the project you need to get the latest release version of the **sbs** build system [from here](https://github.com/lbrugnara/sbs/releases) based on your platform.

### Dependencies

#### Linux
- GCC or Clang

#### Windows

- Clang
- Visual Studio Developer Command Prompt: You will need the Microsoft C++ (MSVC) compiler toolset for the particular architecture you want to compile for. For more information check this Microsoft page: [Build C/C++ code on the command line](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2017).

### Building

Based on your platform run `sbs build linux-gcc-release` or `sbs build win-clang-release` (or the `-debug` version if needed).

If you want to compile with Clang under Linux, you can run:

```bash
sbs build linux-gcc-release -tc=clang
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

Yes, check the `tests` folder and navigate through the different test suites. There you will find usages of the different modules functions. Also projects like [**sbs**](https://github.com/lbrugnara/sbs) and [**Zenit**](https://github.com/lbrugnara/zenit) heavily rely on **fllib**.

#### License?

MIT
