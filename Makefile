all: memgrind.o mymalloc.o
	gcc memgrind.o mymalloc.o -o memgrind

run: memgrind.o mymalloc.o
	gcc memgrind.o mymalloc.o -o memgrind
	./memgrind

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	gcc -c memgrind.c

clean:
	rm -f memgrind *.o
