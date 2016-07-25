CC=gcc
DEBUG=-ggdb
override CFLAGS += -O0 -Wall -Wno-unused-variable -Wno-unused-function -std=c99 -pedantic -fstrict-aliasing $(DEBUG) -DFL_DEBUG

target=debug

FL_OBJECTS=\
	obj/$(target)/src/Std.o 					\
	obj/$(target)/src/Mem.o 					\
	obj/$(target)/src/Cstr.o 					\
	obj/$(target)/src/Array.o 					\
	obj/$(target)/src/containers/Iterator.o 		\
	obj/$(target)/src/containers/Vector.o 			\
	obj/$(target)/src/containers/List.o 			\
	obj/$(target)/src/containers/KeyValuePair.o		\
	obj/$(target)/src/containers/Dictionary.o 		\
	obj/$(target)/src/text/Regex.o

fl: folders $(FL_OBJECTS)
	$(CC) -shared $(CFLAGS) $(FL_OBJECTS) -o build/$(target)/libfl.so
	ar rcs build/$(target)/libfl.a $(FL_OBJECTS)
	@cd tests && cd containers && $(MAKE) $(MFLAGS) --no-print-directory
	@cd tests && cd text && $(MAKE) $(MFLAGS) --no-print-directory
	@cd tests && cd lang && $(MAKE) $(MFLAGS) --no-print-directory

ifneq ($(OS),Windows_NT)
override CFLAGS += -fPIC
obj/$(target)/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
else
obj/$(target)/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
endif

.PHONY: clean folders
clean:	
	@rm -fr obj/
	@rm -fr build/
	@cd tests && cd containers && $(MAKE) --no-print-directory clean
	@cd tests && cd text && $(MAKE) --no-print-directory clean
	@cd tests && cd lang && $(MAKE) --no-print-directory clean


folders:
	@mkdir -p obj/$(target)/src/{lang,containers,text}
	@mkdir -p build/$(target)