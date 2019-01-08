CC=clang
CFLAGS=-Wall -std=c99 -O3
CLINKS=-lm
CFILES=main.c graph.c well1024.c utils.c

levins: main.c
	$(CC) $(CFLAGS) $(CFILES) -o levins $(CLINKS)

clean:
	rm levins
