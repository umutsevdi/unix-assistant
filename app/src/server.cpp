#include "server.h"

#define LINE_SIZE 255
#define BACK_LOG 5

void *sv_handle(void *arg) {
  int newsockfd = ((thread_arg *)arg)->newsockfd;
  int write_pipe = ((thread_arg *)arg)->write_pipe;
  pid_t pid = ((thread_arg *)arg)->pid;
  bool *listening = ((thread_arg *)arg)->listening;

  char buffer[LINE_SIZE]; // buffer of size created
  // allow 4 failed message to arrive
  int fail_counter = 0;
  // while not failing and not exited
  while (fail_counter < 4 && *listening) {
    bzero(buffer, LINE_SIZE); // Clears the buffer
    int n = recv(newsockfd, buffer, LINE_SIZE, 0);
    if (n > 1) {
      printf("%i::%lu\tMessage received: %s\n", pid, pthread_self(), buffer);
      write(write_pipe, &buffer, n);
      if (strncmp(buffer, "exit", 4) == 0) {
        printf("%i::%lu\tExit message received, closing the connection", pid,
               pthread_self());
        *listening = false;
      }
    } else {
      fail_counter++;
      printf("%i::%lu\tError: Failed reading from the socket, strike %d\n", pid,
             pthread_self(), fail_counter + 1);
    }
  }
  printf("%i::%lu\tClosing thread\n", pid, pthread_self());
  close(newsockfd);
  return NULL;
}

void sv_listen(pid_t pid, int write_pipe, int port) {
  int newsockfd;                          // int variable that is used later
  struct sockaddr_in serv_addr, cli_addr; // calling the library struct
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
  int len = sizeof(cli_addr);
  bool listening = true;
  while ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                             (socklen_t *)&len)) &&
         listening) {
    if (newsockfd < 0) {
      printf("%i\tError: Can not accept\n", pid);
    }
    pthread_t thread;

    thread_arg args = thread_arg{pid, newsockfd, write_pipe, &listening};
    if (pthread_create(&thread, NULL, sv_handle, (void *)&args)) {
      printf("Error: Can not create a thread\n");
      exit(EXIT_FAILURE);
    }
  }
  printf("%i\tExiting process", pid);
}
