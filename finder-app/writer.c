// REF: https://www.w3schools.com/c/c_ref_reference.php


#include <stdio.h>     // Defines pointers; stdin, stdout, stderr, fopen()
#include <stdlib.h>    // Memory mgt; exit(), abort(), system()
#include <syslog.h>    // openlog(), syslog(), closelog()
// #include <string.h>
// #include <stdstr.h>    // strlen(), strcpy(), strcat(), strcmp()
// #include <stderr.h>    // Part of <string.h>; returns pointer to string with error msg
// #include <errno.h>     // Global int variable.


int main(int argCount, char *argVector[]) {
    // BASH: cd finder-app/


    printf("\n");
    printf("FILE: writer.c...");
    printf("\n");


    // Open log file
    printf("Opening log...");
    openlog("msWriterLog", LOG_PID, LOG_USER);
    printf("...DONE\n");


    printf("\n");
    printf("NOTE: Log-file in folder /var/log \n");
    printf("      sudo tail -f /var/log/syslog\n");    // Show last 10 lines; continue to monitor and output.


    // Count number of arguments inputted by user
    if (argCount != 3) {
        syslog(LOG_ERR, "Number of inputs NOT EQUAL to 3. Aborting...");
        printf("\n");
        printf("ERROR: Two inputs are required; 'writefile' and 'writestr'.\n");
        printf("       Ex. ./writer writerOutput.txt \"Add this string\"\n");
        printf("       Aborting...\n");
        printf("\n");
        printf("Closing log...");
        closelog();
        printf("...DONE\n");
        printf("\n");
        return 1;
    }


    // Assign variables to inputs
    char *writefile = argVector[1];
    char *writestr = argVector[2];
    printf("\n");
    printf("writefile: %s\n", writefile);
    printf("writestr : %s\n", writestr);
    printf("\n");


    // Open writefile
    printf("Opening writefile...");
    FILE *openedFile = fopen(argVector[1], "a");    // current directory; w/a/r
    if (openedFile == NULL) {
        syslog(LOG_ERR, "Failed to open file %s...aborting...", writefile);
        printf("\nERROR; Failed to open file %s...aborting...\n\n", writefile);
        closelog();
        return 1;
    }
    printf("...DONE\n");


    // Write writestr to openedFile
    fprintf(openedFile, "\n%s\n", argVector[2]);
    syslog(LOG_DEBUG, "Writing \"%s\" to \"%s\".", writestr, writefile);


    // Close openedFile
    printf("\n");
    printf("Closing writefile...");
    fclose(openedFile);
    printf("...DONE\n");


    // Close syslog
    printf("Closing log...");
    closelog();
    printf("...DONE\n");


    printf("\n");


    return 0;


    // BASH: gcc -o writer writer.c


}



