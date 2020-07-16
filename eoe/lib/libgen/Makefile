CC = gcc
CFLAGS = -std=c99
OPTFLAGS = -O2 -Wall

$(shell mkdir -p build/src)

TARGET=build/libgen.a
CFILES = src/mkdirp.c
OBJS := $(foreach f, $(CFILES:.c=.o),build/$f)

default: $(TARGET) $(OBJS)

build/libgen.a: $(OBJS)
	ar rcs build/libgen.a $(OBJS)

build/src/%.o: src/%.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf build

