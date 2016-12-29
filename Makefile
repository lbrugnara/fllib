CC=gcc
AR=ar
override DEBUG += -ggdb -DFL_DEBUG
override CFLAGS += -O0 -Wall -Wno-unused-variable -Wno-unused-function -std=c99 -pedantic -fstrict-aliasing
# Macros for target tests
TESTS=

ifneq ($(OS),Windows_NT)
	override CFLAGS += -fPIC
	override LIBS += -lpthread
endif

ifneq (,$(findstring FL_PTHREADS,$(CFLAGS)))
	override LIBS += -pthread
endif

# Default TARGET is debug (debut|release)
TARGET=debug

ifeq ($(TARGET),debug)
	override CFLAGS += $(DEBUG)
endif

# Linkage
VALID_LINKAGES := static shared
LINKAGE=static
FL_STATIC_LIB=libfl.a
FL_SHARED_LIB=libfl.so

ifeq ($(OS),Windows_NT)
	override FL_SHARED_LIB=libfl.dll
endif

ifeq ($(filter $(LINKAGE), $(VALID_LINKAGES)),)
	$(error $(LINKAGE) is not a valid linkage parameter. Please specify a valid one.)
endif

# Core modules
FL_OBJECTS=\
	obj/$(TARGET)/src/threading/Thread.o 			\
	obj/$(TARGET)/src/Std.o 						\
	obj/$(TARGET)/src/Mem.o 						\
	obj/$(TARGET)/src/Cstr.o 						\
	obj/$(TARGET)/src/Array.o 						\
	obj/$(TARGET)/src/File.o 						\
	obj/$(TARGET)/src/containers/Iterator.o 		\
	obj/$(TARGET)/src/containers/Vector.o 			\
	obj/$(TARGET)/src/containers/List.o 			\
	obj/$(TARGET)/src/containers/KeyValuePair.o		\
	obj/$(TARGET)/src/containers/Dictionary.o 		\
	obj/$(TARGET)/src/text/Regex.o 					\
	obj/$(TARGET)/src/os/System.o 					\
	obj/$(TARGET)/src/os/Signal.o

ifeq ($(OS),Windows_NT)
	override FL_OBJECTS += obj/$(TARGET)/src/os/WinEx.o
endif

# Test modules
FL_TEST_OBJECTS=\
	obj/$(TARGET)/tests/Main.o \
	obj/$(TARGET)/tests/Test.o \
	obj/$(TARGET)/tests/Std.o \
	obj/$(TARGET)/tests/File.o \
	obj/$(TARGET)/tests/Cstr.o 

ifeq ($(LINKAGE),static)
	# Creates the .a file
	COMMAND_COMPILE_FL=$(AR) rcs build/$(TARGET)/$(FL_STATIC_LIB) $(FL_OBJECTS)
	# Compiles the tests using the .a file generated by fllib target
	COMMAND_COMPILE_TEST=\
		@mkdir -p $(dir $@) && mkdir -p tests/build/$(TARGET); \
		$(CC) $(CFLAGS) $(LIBS) $(FL_TEST_OBJECTS) -o tests/build/$(TARGET)/tests build/$(TARGET)/$(FL_STATIC_LIB)

else ifeq ($(LINKAGE),shared)
	# Creates a shared library (so or dll)
	COMMAND_COMPILE_FL=\
		$(CC) -shared $(CFLAGS) $(FL_OBJECTS) -o build/$(TARGET)/$(FL_SHARED_LIB)
	# Compiles the tests using the shared library generated by fllib target.
	# (Windows) Copy the .dll into the tests directory.
	COMMAND_COMPILE_TEST=\
		@mkdir -p $(dir $@) && mkdir -p tests/build/$(TARGET); \
		$(CC) $(CFLAGS) $(LIBS) -L./build/$(TARGET) -lfl $(FL_TEST_OBJECTS) -o tests/build/$(TARGET)/tests; \
		cp build/$(TARGET)/$(FL_SHARED_LIB) tests/build/$(TARGET)/
endif

fllib: folders $(FL_OBJECTS)
	$(COMMAND_COMPILE_FL)

tests: fllib $(FL_TEST_OBJECTS)
	$(COMMAND_COMPILE_TEST)

obj/$(TARGET)/src/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

obj/$(TARGET)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIBS) $(TESTS) -c $< -o $@ -I./include -I./src

.PHONY: clean folders
clean:	
	@rm -fr {obj,build}/
	@rm -fr tests/{obj,build}/

folders:
	@mkdir -p obj/$(TARGET)/src
	@mkdir -p build/$(TARGET)