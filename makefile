CFLAGS = -Wall -pedantic -O3 
SANITIZE_FLAGS = -fsanitize=address

ifeq ($(SANITIZE),1)
    CFLAGS += $(SANITIZE_FLAGS)
endif

main: main.o server.o processPersonality.o cJSON.o
	gcc $(CFLAGS) -o main main.o server.o processPersonality.o cJSON.o

main.o: main.c server.h processPersonality.h
	gcc $(CFLAGS) -c main.c

server.o: server.c server.h processPersonality.h
	gcc $(CFLAGS) -c server.c

processPersonality.o: processPersonality.c processPersonality.h cJSON/cJSON.h
	gcc $(CFLAGS) -c processPersonality.c

cJSON.o: cJSON/cJSON.c cJSON/cJSON.h
	gcc $(CFLAGS) -c cJSON/cJSON.c -o cJSON.o

clean:
	rm -f main
	rm -f *.o
	rm -f a.out