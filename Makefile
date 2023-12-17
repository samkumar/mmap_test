CC = clang
CFLAGS = -Ofast
LDFLAGS =

PROGRAM = mmap_test

.PHONY: all clean

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o
	$(CC) $(LDFLAGS) $+ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $+ -o $@

clean:
	rm -f $(PROGRAM) *.o