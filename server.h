#ifndef SERVER_H
#define SERVER_H

#define PORT 9999
#define ADDRESS "127.0.0.1"
#define MAXCLIENTS 10
#define MAXBUFFER 500

int runServer();
void sigintHandler();
void sigusr1Handler();
void sigusr2Handler();
void clientRequestHandler();
void serveRequest();
void readMessageFromClient(char buffer[]);
void writeMessageToClient(const char *message);

#endif