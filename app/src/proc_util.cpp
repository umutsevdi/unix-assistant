#include "proc_util.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

char **proc_split(const char *const string, int len, int *argc) {
  int i = 1;
  // length describes the size of the args array
  int length = 1;
  char **args;

  // duplicate string
  char *buffer = (char *)malloc(len * sizeof(char));
  memcpy(buffer, string, strlen(string));

  // replace all space characters and count them
  while (buffer[i] != '\0') {
    if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') {
      length++;
      buffer[i] = ' ';
    }
    i++;
  }

  args = (char **)malloc(sizeof(char *) * (length + 1));

  char *tok = strtok(buffer, " ");
  i = 0;
  while (tok != NULL) {
    args[i] = strdup(tok);
    i++;
    tok = strtok(NULL, " \n");
  }
  // write NULL to the last element, requirement for `execvp`
  args[i] = NULL;
  // write argument count to argc
  *argc = length;
  // return the array
  free(buffer);
  return args;
}

char *proc_exec(char *const program, int len) {
  int arg_len;
  char *const *prog_arg = proc_split(program, len, &arg_len);

  // printf("%s(", prog_arg[0]);
  // int i = 1;
  // for (; i < arg_len; i++) {
  //   printf("%s, ", prog_arg[i]);
  // }
  // printf(") :\n");

  int pipefd[2]; // creating pipe to communicate between parent and
  char pipebuffer[PROC_UTIL_BUFFER_SIZE];
  pipe(pipefd);
  pid_t pid = fork();
  if (pid == 0) {
    // set stderr and stdout targets as pipe's write end
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    dup2(pipefd[1], 2);
    // create and run the new process and
    // close the child process
    execvp(prog_arg[0], prog_arg);
  } else {
    close(pipefd[1]);
    wait(&pid);
    // read returns buffer_size
    int buffer_size = read(pipefd[0], pipebuffer, PROC_UTIL_BUFFER_SIZE);
    pipebuffer[buffer_size] = '\0';

    char *output = (char *)malloc(buffer_size);
    strncpy(output, pipebuffer, buffer_size);

    // free prog_arg since all elements are already duplicated
    free(prog_arg[0]);
    return output;
  }
  return 0;
}
