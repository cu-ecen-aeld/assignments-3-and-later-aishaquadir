#include <stdlib.h>
#include "systemcalls.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

#define _XOPEN_SOURCE

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/

bool do_system(const char *cmd)
{
    /* 
    system():
        - return true (exit code) if success
        - return false (-1) if failed
    */

    int sysResult = system(cmd);
    return (sysResult == 0);
} 

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    // this line is to avoid a compile warning before your implementation is complete and may be removed
    //command[count] = command[count];

    /*
    *   Execute a system command by calling fork, execv(),
    *   and wait instead of system (see LSP page 161).
    *   Use the command[0] as the full path to the command to execute
    *   (first argument to execv), and use the remaining arguments
    *   as second argument to the execv() command.
    */

    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) // Error
        return -1;
    else if (pid == 0)  {
        // Child
        execv(command[0], command);
        exit(-1);
    }

    // Parent 
    pid_t tpid;
    do {
        tpid = wait(&status);
    } while(tpid != pid);

    return WEXITSTATUS(status) == 0;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/

bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char*);
    }
    command[count] = NULL;

    // this line is to avoid a compile warning before your implementation is complete and may be removed
    // command[count] = command[count];

    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) // error
        return -1;
    else if (pid == 0) {
        // Child
        int fd = open(outputfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);
        close(fd);

        execv(command[0], command);

        exit (-1);
    }
    
    // Parent 
    pid_t tpid;
    do {
        tpid = wait(&status);
    } while(tpid != pid);

    return WEXITSTATUS(status) == 0;
} 