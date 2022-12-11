#include "display.h"
#include "server.h"
#include <cstdio>
#include <unistd.h>

#define PORT 8081

int main(int argc, char **argv) {

  int sv_pipe[2];
  pipe(sv_pipe);

  pid_t pid = fork();

  if (!pid) {
    if (!disp_setup()) {
      printf("%i\tStarting up display\n", getpid());
      close(sv_pipe[1]);
      disp_loop(sv_pipe[0]);
      disp_close();
    } else {
      perror("Failed to setup OLED display\n");
    }
    // parent
  } else {
    close(sv_pipe[0]);
    printf("%i\tStarting TCP server at :%d\n", getpid(), PORT);
    sv_listen(sv_pipe[1], PORT);
  }

  return 0;
}
