#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAX_LEN 1024

void getCmdArguments(char **preArgv, int preArgc, char *newArgv, char **argv)
{
    int k = 0;
    for (int i = 0; i < preArgc; i++)
    {
        argv[k] = malloc(strlen(preArgv[i]) + 1);
        memcpy(argv[k++], preArgv[i], strlen(preArgv[i]) + 1);
    }
    argv[k] = malloc(strlen(newArgv) + 1);
    memcpy(argv[k++], newArgv, strlen(newArgv) + 1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Forget to pass argument\n");
        exit(1);
    }

    char lstArg[MAX_LEN];
    int i = 0;
    char curChar;
    int isValid = 0;

    while (read(0, &curChar, 1) > 0)
    {
        if (curChar == '\n')
        {
            if (isValid)
            {
                i = 0;
                isValid = 0;
                continue;
            }
            lstArg[i] = 0;
            i = 0;

            int pid = fork();
            if (pid == 0)
            {
                int curArgc = argc;
                char *curArgv[MAXARG];

                getCmdArguments(argv + 1, argc - 1, lstArg, curArgv);
                curArgv[curArgc] = 0;

                exec(curArgv[0], curArgv);
                exit(0);
            }
            else
            {
                wait((int *)0);
            }
        }
        else
        {
            if (!isValid && i >= MAX_LEN - 1)
            {
                isValid = 1;
                printf("Too many arguments...\n");
            }
            if (!isValid)
            {
                lstArg[i++] = curChar;
            }
        }
    }

    exit(0);
}