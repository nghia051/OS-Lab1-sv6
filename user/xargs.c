#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAXARGS 10

int main(int argc, char *argv[]) {
    char *args[MAXARGS];
    char line[512];
    int i;

    // Check for correct usage
    if(argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }

    // Prepare the command to run
    for(i = 1; i < argc && i < MAXARGS - 1; i++) {
        args[i - 1] = argv[i];
    }

    // Read lines from the standard input
    while(gets(line, sizeof(line))) {
        // Remove newline character
        char *newline = strchr(line, '\n');
        if(newline) {
            *newline = 0;
        }

        // Append the line to the command's arguments
        args[i - 1] = line;
        args[i] = 0;

        // Run the command
        if(fork() == 0) {
            exec(args[0], args);
            fprintf(2, "xargs: failed to execute %s\n", args[0]);
            exit(1);
        } else {
            wait((int*)0);
        }
    }

    exit(0);
}