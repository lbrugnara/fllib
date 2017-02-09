# Make and build variables
CC=gcc
DEBUG=-ggdb
override CFLAGS += -O0 -Wall -Wno-unused-variable -Wno-unused-function -std=c99 -pedantic -fstrict-aliasing $(DEBUG)

target=debug
linkage=static
name=test

FL_OBJECTS=obj/$(target)/main.o	obj/$(target)/fl_vector_tests.o



ifeq ($(linkage),static)
test: folders $(FL_OBJECTS)
	@echo "Compiling test with static libfl.a"
	@$(CC) $(CFLAGS) $(FL_OBJECTS) -o build/$(target)/$(name) ../../build/$(target)/libfl.a
else ifeq ($(linkage),shared)
test: folders $(FL_OBJECTS)
	@echo "Compiling test with shared libfl.so"
	@$(CC) $(CFLAGS) -L../../build/$(target) $(FL_OBJECTS) -o build/$(target)/$(name) -lfl
else
test:
	echo "Please specify linkage"
endif


obj/$(target)/%.o: ./%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean folders
clean:	
	@rm -fr obj/
	@rm -fr build/


folders:
	@mkdir -p obj/$(target)/src/{lang,containers,text}
	@mkdir -p build/$(target)