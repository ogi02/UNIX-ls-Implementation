all: helpers.o ls.o
	gcc -o ls helpers.o ls.o

helpers.o: helpers.h helpers.c
	gcc -c helpers.c 

ls.o: ls.c
	gcc -c ls.c

clean:
	rm *.o ls