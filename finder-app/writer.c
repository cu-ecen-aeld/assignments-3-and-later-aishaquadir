#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv)
    // int argc = number of arguments given to the command line, passed into c main function
    // char**argv = the arguments
    // char* = array of characters
    // char** = array of strings
    {
        openlog(NULL,0,LOG_USER);

        char* writefile;
        char* writestr;
        FILE *fPointer; 

        writefile = argv[1];
        writestr = argv[2];

        if (argc != 3) 
        {
            syslog(LOG_ERR, "Error: Number of arguments required = 2. You have supplied %d arguments.", argc-1);
            return 1; // return : exit main()
        }

        fPointer =  fopen(writefile, "w+");
        if (fPointer == NULL)
        {
            syslog(LOG_ERR, "Error: File %s cannot be opened.", writefile);
            return 1;
        }

        int writeError = fprintf(fPointer, "%s", writestr);

        if (writeError < 0)
        {
            printf("%d",writeError);
            syslog(LOG_ERR, "Error writing string to file.");
            return 1;
        }

        fclose(fPointer);
    }