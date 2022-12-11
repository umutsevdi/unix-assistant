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
    close(sv_pipe[0]);
    printf("%i\tStarting TCP server at :%d\n", getpid(), PORT);
    sv_listen(getpid(),sv_pipe[1], PORT);
    // parent
  } else {
    if (!disp_setup(getpid())) {
      printf("%i\tStarting up display\n", getpid());
      close(sv_pipe[1]);
      disp_loop(sv_pipe[0]);
      disp_close();
    } else {
      printf("%i\tFaield to setup OLED display\n", getpid());
    }
  }

  return 0;
}
