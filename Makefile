main: main.c
	clang -O3 main.c graph.c well1024.c utils.c -o main -lm

