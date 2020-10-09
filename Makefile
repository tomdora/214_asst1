all: memgrind.c mymalloc.o
	gcc memgrind.c mymalloc.o

mymalloc.o: mymalloc.c
	gcc -c mymalloc.c

t: a.out
	./a.out

clean: a.out mymalloc.o
	rm a.out
	rm mymalloc.o
	
