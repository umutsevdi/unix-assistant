#include "proc_util.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

char *const *proc_parse(char *program) {
  char *ptr = strsep(&program, " ");
  printf("%s,%s\n", ptr, program);
  char *str_arr[2];
  str_arr[0] = ptr;
  // if (program == NULL) {
  //   program = (char *)malloc(sizeof(char) * 2);
  //   program[0] = ' ';
  //   program[1] = '\0';
  // }
  // printf("program arguments are {%s}\n", program);
  str_arr[1] = program;
  char **str_ptr = (char **)malloc(sizeof(char *) * 2);
  memcpy(str_ptr, str_arr, sizeof(str_arr));
  return str_ptr;
}

int proc_exec(char *program, char *str) {
  char *program_str_immutable = (char *)malloc(sizeof(program));
  strcpy(program_str_immutable, program);
  char *const *prog_arg = proc_parse(program_str_immutable);

  printf("run %s#%s\n", prog_arg[0], prog_arg[1]);

  int pipefd[2]; // creating pipe to communicate between parent and
  char pipebuffer[1024];
  pipe(pipefd);
  pid_t pid = fork();
  if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    dup2(pipefd[1], 2);
    // create and run the new process and
    // close the child process
    if (prog_arg[1] == NULL) {
      execlp(prog_arg[0], prog_arg[0], NULL);
    } else {
      execvp(prog_arg[0], &prog_arg[1]);
    }
  } else {
    wait(&pid);
    close(pipefd[1]);
    // read returns buffer_size
    int buffer_size = read(pipefd[0], pipebuffer, sizeof(pipebuffer));
    pipebuffer[buffer_size] = '\0';
    strcpy(str, pipebuffer);
    free(prog_arg[0]);
    return buffer_size;
  }
  return 0;
}
