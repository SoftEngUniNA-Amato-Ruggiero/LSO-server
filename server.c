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
    signal(SIGINT, &sigintHandler); 
    signal(SIGUSR1, &sigusr1Handler);
    signal(SIGUSR2, &sigusr2Handler);
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
        raise(SIGUSR1);
    }

    printf("Listening on welcome socket...\n");
    int listenResult = listen(welcomeSocket, MAXCLIENTS);
    if (listenResult < 0) {
        perror("listen failed");
        raise(SIGUSR1);
    }
    
    printf("(press Ctrl+C to stop)\n\n");

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
        raise(SIGUSR1);
    }
    else if (childPid == 0)
    {
        close(welcomeSocket);
        serveRequest();
        close(clientSocket);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(clientSocket);
    }
}

void serveRequest(){
    char buffer[MAXBUFFER];
    readMessageFromClient(buffer);
    printf("Received message:\n%s ", buffer);

    char *personality = (char*) malloc(MAXBUFFER * sizeof(char));
    if (personality == NULL){
        perror("Error allocating memory for personality\n\n");
        raise(SIGUSR2);
    }
    memset(personality, 0, MAXBUFFER * sizeof(char));

    int processPersonalityResult = processPersonality(buffer, personality);
    if (processPersonalityResult != 0){
        perror("Error processing personality\n\n");
        free(personality);
        personality = NULL;
        raise(SIGUSR2);
    }
    
    writeMessageToClient(personality);

    free(personality);
    personality = NULL;
}

void readMessageFromClient(char buffer[]){
    int recvResult = recv(clientSocket, buffer, MAXBUFFER, 0);
    if (recvResult < 0) {
        perror("read failed");
        raise(SIGUSR2);
    }
    return;
}

void writeMessageToClient(const char *message){
    if (message == NULL){
        perror("Error writing message to client\n\n");
        raise(SIGUSR2);
    }
    else {
        unsigned long messageLength = sizeof(char)*(strnlen(message, MAXBUFFER));
        int sendResult = send(clientSocket, message, messageLength, 0);
        if (sendResult < 0) {
            perror("write failed");
            raise(SIGUSR2);
        }
    }
}

void sigintHandler(){
    close(welcomeSocket);
    printf("Stopped.\n");
    exit(EXIT_SUCCESS);
}

void sigusr1Handler(){
    close(welcomeSocket);
    exit(EXIT_FAILURE);
}

void sigusr2Handler(){
    close(clientSocket);
    exit(EXIT_FAILURE);
}