#ifndef __PROC_UTIL__
#define __PROC_UTIL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * executes given command and writes it's output to the str
 * @param program program with arguments to execute
 * @param str buffer to write the output
 * @return sizeo f the str
 *
 */
int proc_exec(char *program, char *str);

/**
 * parses given command into two string
 * @param program program with arguments
 * @return an array of string that contains two elements
 * name of the program, and it's arguments
 *
 */
char *const *proc_parse(char *program);
#endif
