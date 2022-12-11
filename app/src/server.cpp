#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define LINE_SIZE 255
#define BACK_LOG 5

void sv_listen(pid_t pid, int write_pipe, int port) {
  int n;
  int newsockfd, clilen;                  // int variable that is used later
  struct sockaddr_in serv_addr, cli_addr; // calling the library struct
  char buffer[LINE_SIZE];                 // buffer of size created
                                          //
  // 1st IP Address 2nd TCP Concept 3rd Socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("%i\tError: Socket creation failed\n", pid);
    exit(0);
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("%i\tError: Can not bind to socket\n", pid);
    exit(0);
  }

  if ((listen(sockfd, BACK_LOG)) != 0) {
    printf("%i\tError: Listening failed\n", pid);
    exit(0);
  }
  clilen = sizeof(cli_addr);

  int listening = 1;
  while ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                             (socklen_t *)&clilen)) &&
         listening) {
    if (newsockfd < 0) {
      printf("%i\tError: Can not accept\n", pid);
    }
    if (!fork()) {
      pid = getpid();
      // allow 4 failed message to arrive
      int fail_counter = 0;
      while (fail_counter < 4) {
        bzero(buffer, LINE_SIZE); // Clears the buffer
        n = recv(newsockfd, buffer, LINE_SIZE, 0);
        if (n < 0) {
          printf("%i\tError: Can not read from socket, closing the socket\n",
                 pid);
          listening = 0;
          break;
        } else if (n > 1) {
          printf("%i\tMessage received: %s\n", pid, buffer);
          // Buffer Stores the msg sent by the client
          write(write_pipe, &buffer, n);
          if (strncmp(buffer, "exit", 4) == 0) {
            printf("%i\tClosing connection", pid);
            close(newsockfd);
            listening = 0;
            break;
          }
        } else {
          fail_counter++;
        }
      }
      if (fail_counter >= 4) {
        printf("%i\tError: Fail threshold is exceeded", pid);
      }
    }
  }
  printf("%i\tExiting process", pid);
}
