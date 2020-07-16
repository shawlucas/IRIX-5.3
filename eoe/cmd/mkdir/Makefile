CC = gcc
CFLAGS = -std=c99
OPTFLAGS = -O2 -Wall
LIB = $(IRIX_SRC_ROOT)/eoe/lib/libgen/build/
LIBS=$(LIB)/libgen.a
$(shell mkdir -p build)
$(shell mkdir -p bin)

TARGET=bin/mkdir
CFILES = mkdir.c
OBJS := $(foreach f, $(CFILES:.c=.o),build/$f)

default: $(TARGET) $(OBJS)

bin/mkdir: $(OBJS)
	$(CC) $(OBJS) -L$(LIBS) -o bin/mkdir $(LIBS)

build/%.o: %.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf build
	rm -rf bin

