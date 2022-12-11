#include "server.h"

#define LINE_SIZE 255
#define BACK_LOG 5

void sv_listen(int write_pipe, int port) {
    int n;
    int sockfd, newsockfd, clilen;          // int variable that is used later
    struct sockaddr_in serv_addr, cli_addr; // calling the library struct
    char buffer[LINE_SIZE];                 // buffer of size created
    //
    // 1st IP Address 2nd TCP Concept 3rd Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("ERR>> socket creation failed");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // convert and use port number
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERR>> can not bind to socket");
    }

    listen(sockfd, BACK_LOG);
    clilen = sizeof(cli_addr);

    while ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
                               (socklen_t * ) & clilen))) {
        if (newsockfd < 0) {
            perror("ERR>> can not accept");
        }
        if (!fork()) {
            while (1) {
                bzero(buffer, LINE_SIZE); // Clears the buffer
                n = recv(newsockfd, buffer, LINE_SIZE, 0);
                if (n < 0) {
                    perror("ERR>> can not read from socket");
                }
                // Buffer Stores the msg sent by the client
                write(write_pipe, &buffer, strnlen(buffer, LINE_SIZE));
            }
        }
    }
}
