main: server.o
	gcc main.c server.o -o main

server.o: server.c server.h
	gcc -c server.c

clean:
	rm -f main
	rm -f *.o