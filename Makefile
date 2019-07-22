all: memgrind

mymalloc: mymalloc.c
	gcc -g -Wall -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c
	gcc -g -Wall -c mymalloc.c
	gcc -g -Wall -o memgrind memgrind.c mymalloc.o

clean:
	rm -f mymalloc.o memgrind
