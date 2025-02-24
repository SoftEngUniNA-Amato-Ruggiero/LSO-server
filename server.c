#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define PORT 9999
#define ADDRESS "127.0.0.1"
#define MAXCLIENTS 10
#define MAXBUFFER 999999

int welcomeSocket;
int clientSocket;
struct sockaddr_in serverAddress;
struct sockaddr_in clientAddress;
socklen_t clientAddressLength = sizeof(struct sockaddr_in);

void signalHandler();
void clientRequestHandler();

int main() {
    signal(SIGINT, signalHandler); 
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    memset(&clientAddress, 0, sizeof(struct sockaddr_in));

    printf("Creating welcome socket...\n");
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Setting welcome socket address...\n");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    printf("Binding welcome socket...\n");
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
            continue;
        }
        printf("Client connected\n");
        clientRequestHandler();
    } while (1);

    return 0;
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

        // read message from client
        char buffer[MAXBUFFER];
        int readResult = read(clientSocket, buffer, MAXBUFFER);
        if (readResult < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        // execute server code
        printf("Received message:\n%s\n", buffer);

        // close client socket
        close(clientSocket);
        exit(0);
    }
    else
    {
        close(clientSocket);
    }
}

void signalHandler(){
    close(welcomeSocket);
    printf("\nStopped.\n");
    exit(0);
}
