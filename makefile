CFLAGS = -Wall -pedantic -O3 -fsanitize=address

main: main.c server.o 
	gcc $(CFLAGS) -o main main.c server.o processPersonality.o cJSON.o

server.o: server.c server.h processPersonality.o 
	gcc $(CFLAGS) -c server.c

processPersonality.o: processPersonality.c processPersonality.h cJSON.o
	gcc $(CFLAGS) -c processPersonality.c

cJSON/cJSON.o: cJSON.c cJSON.h
	gcc $(CFLAGS) -c cJSON.c

clean:
	rm -f main
	rm -f *.o
	rm -f a.out