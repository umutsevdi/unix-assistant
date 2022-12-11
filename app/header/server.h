#ifndef __SV__
#define __SV__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * Listens TCP messages form given port and writes them to the pipe
 * @param write_pipe, write end of the pipe to deliver messages
 * @param int port
 */
void sv_listen(pid_t pid, int write_pipe, int port);

#endif
