#ifndef SERVER_H
#define SERVER_H

#define PORT 9999
#define ADDRESS "127.0.0.1"
#define MAXCLIENTS 10
#define MAXBUFFER 999999

int runServer();
void signalHandler();
void clientRequestHandler();
void serveRequest();
char* readClientMessage(char buffer[]);
void writeClientMessage(char message[]);

#endif