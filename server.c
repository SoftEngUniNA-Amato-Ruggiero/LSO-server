#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>

#include "processPersonality.h"

int welcomeSocket;
int clientSocket;
struct sockaddr_in serverAddress;
struct sockaddr_in clientAddress;
socklen_t clientAddressLength = sizeof(struct sockaddr_in);

int runServer() {
    signal(SIGINT, &signalHandler); 
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    memset(&clientAddress, 0, sizeof(struct sockaddr_in));

    printf("Creating welcome socket...\n");
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Setting server address...\n");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    printf("Binding welcome socket to server address...\n");
    int bindResult = bind(welcomeSocket, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr));
    if (bindResult < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening on welcome socket...\n");
    int listenResult = listen(welcomeSocket, MAXCLIENTS);
    if (listenResult < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("(press Ctrl+C to stop)\n");
    do{
        clientSocket = accept(welcomeSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("accept failed");
        } else {
            printf("Client connected\n");
            clientRequestHandler();
        }
    } while (1);
}

void clientRequestHandler() {
    pid_t childPid = fork();
    if (childPid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (childPid == 0)
    {
        close(welcomeSocket);
        serveRequest();
        close(clientSocket);
        exit(0);
    }
    else
    {
        close(clientSocket);
    }
}

void serveRequest(){
    char buffer[MAXBUFFER];
    readMessageFromClient(buffer);
    printf("Received message:\n%s\n", buffer);

    char *personality = processPersonality(buffer);
    writeMessageToClient(personality);
}

char* readMessageFromClient(char buffer[]){
    int readResult = read(clientSocket, buffer, MAXBUFFER);
    if (readResult < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

void writeMessageToClient(const char *message){
    int writeResult = write(clientSocket, message, sizeof(char)*(strlen(message)));
    if (writeResult < 0) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }
}

void signalHandler(){
    close(welcomeSocket);
    printf("\nStopped.\n");
    exit(0);
}
