#ifndef __SV__
#define __SV__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct thread_arg {
  pid_t pid;
  int newsockfd;
  int write_pipe;
  bool *listening;
};

/**
 * Listens TCP messages form given port and writes them to the pipe
 * @param write_pipe, write end of the pipe to deliver messages
 * @param int port
 */
void sv_listen(pid_t pid, int write_pipe, int port);

/**
 * Handles the connection for a single user on a separate thread
 * @param arg {@link thread_arg} thread arguments
 *
 */
void *sv_handle(void *arg);
#endif
