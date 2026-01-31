#include "systemcalls.h"
#include <stdio.h>          // ssolusa;
#include <stdlib.h>         // ssolusa; for system()
#include <stdbool.h>        // ssolusa;
#include <unistd.h>         // ssolusa; for fork(), exec()
#include <fcntl.h>          // ssolusa; for open()
#include <errno.h>          // ssolusa; for errors
#include <sys/wait.h>       // ssolusa; for waitpid()
#include <stdarg.h>         // ssolusa; for va_list



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
     * TODO  add your code here
     *  Call the system() function with the command set in the cmd
     *   and return a boolean true if the system() call completed with success
     *   or false() if it returned a failure
    */
    // ssolusa START
    printf("\n***********************************************\n");
    printf("systemcalls.c...do_system()...\n");
    int returnVal = system(cmd);
    if (returnVal == 0) {
        printf("system() completed successfully.\n");
        printf("END\n");
        return true;
    } else {
        printf("ERROR: system() failed.\n");
        printf("       returnVal = %d\n", returnVal);
        printf("       Child process could not be created, or...\n");
        printf("       its status could not be retrieved.\n");
        printf("       errno set to indicate error.\n");
        printf("END\n\n");
        return false;
    }
    // ssolusa END
    return true;
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
    // command[count] = command[count];  // this line is to avoid a compile warning before your implementation is complete and may be removed
    va_end(args);
    /*
     * TODO:
     *   Execute a system command by calling fork, execv(),
     *   and wait instead of system (see LSP page 161).
     *   Use the command[0] as the full path to the command to execute
     *   (first argument to execv), and use the remaining arguments
     *   as second argument to the execv() command.
     *
    */
    // ssolusa START
    printf("\n***********************************************\n");
    printf("systemcalls.c...do_exec()...\n");
    pid_t pid = fork();
    if (pid == -1) {
        printf("ERROR: fork() failed.\n");
        printf("       pid = %d\n", (int)pid);
        printf("       No child process is created.\n");
        printf("       errno = %d\n", errno);
        printf("END\n\n");
        return false;
    }
    if (pid == 0) {
        printf("fork() completed successfully...continuing...\n");
        execv(command[0], command);    // returns value if fails
        printf("ERROR: execv() failed.\n");
        printf("       errno = %d\n", errno);
        printf("END\n\n");
        _exit(EXIT_FAILURE); //return false;
    }
    if (pid > 0) {
        int status;
        pid_t waitpidReturn = waitpid(pid, &status, 0);
        if (waitpidReturn == pid) {
            printf("waitpid() completed successfully.\n");
            return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
        } else {
            printf("ERROR: waitpid() failed.\n");
            printf("       waitpidReturn = %d\n", (int)waitpidReturn);
            printf("       errno = %d\n", errno);
            printf("END\n\n");
            return false;
        }
    }
    // ssolusa END
    return true;
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
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // command[count] = command[count];   // this line is to avoid a compile warning before your implementation is complete and may be removed
    va_end(args);
    /*
     * TODO
     *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
     *   redirect standard out to a file specified by outputfile.
     *   The rest of the behaviour is same as do_exec()
     *
    */
    // ssolusa START
    printf("\n***********************************************\n");
    printf("systemcalls.c...do_exec_redirect()...\n");
    pid_t pid = fork();
    if (pid == -1) {
        printf("ERROR: fork() failed.\n");
        printf("       pid = %d\n", (int)pid);
        printf("       No child process is created.\n");
        printf("       errno = %d\n", errno);
        printf("END\n\n");
        return false;
    }
    if (pid == 0) {
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            _exit(EXIT_FAILURE); // abort();
        }
        int dupTwoReturn = dup2(fd, STDOUT_FILENO);
        if (dupTwoReturn == -1) {
            close(fd);
            _exit(EXIT_FAILURE); // abort();
        }
        close(fd);
        execv(command[0], command);    // returns value if fails
        _exit(EXIT_FAILURE); // abort();
    }
    if (pid > 0) {
        int status;
        pid_t waitpidReturn = waitpid(pid, &status, 0);
        if (waitpidReturn == pid) {
            printf("waitpid() completed successfully.\n");
            printf("END\n\n");
            return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
        } else {
            printf("ERROR: waitpid() failed.\n");
            printf("       waitpidReturn = %d\n", (int)waitpidReturn);
            printf("       errno = %d\n", errno);
            printf("END\n\n");
            return false;
        }
    }
    // ssolusa END
    return true;
}
