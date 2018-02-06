build: Planificator.o
	gcc -Wall Planificator.o -o planificator
planificator.o: Planificator.c
	gcc -c -Wall Planificator.c
clean:
	rm -f *.o planificator
