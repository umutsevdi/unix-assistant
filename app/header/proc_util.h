#ifndef __PROC_UTIL__
#define __PROC_UTIL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROC_UTIL_BUFFER_SIZE 256

/**
 * executes given command and writes it's stdout and stderr to the output.
 * Output is allocated by the function
 * @param program program with arguments to execute
 * @param len length of the string
 * @return output of the program
 *
 */
char *proc_exec(char *const program, int len);

/**
 * parses given string as program name and arguments and writes to an array
 * @param string a string that contains both program name and the arguments
 * @param len    length of the string
 * @param argc   an integer pointer that will be filled by the size of the
 * array that's been returned
 * @return an array that contains program and the arguments
 */
char **proc_split(const char *const string, int len, int *argc);

#endif
