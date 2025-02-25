CFLAGS = -Wall -pedantic -O3 -fsanitize=address

main: main.o server.o processPersonality.o cJSON.o
	gcc $(CFLAGS) -o main main.c server.o processPersonality.o cJSON.o

main.o: main.c server.h processPersonality.h
	gcc $(CFLAGS) -c main.c

server.o: server.c server.h processPersonality.h
	gcc $(CFLAGS) -c server.c

processPersonality.o: processPersonality.c processPersonality.h cJSON.h
	gcc $(CFLAGS) -c processPersonality.c

cJSON.o: cJSON.c cJSON.h
	gcc $(CFLAGS) -c cJSON.c

clean:
	rm -f main
	rm -f *.o
	rm -f a.out