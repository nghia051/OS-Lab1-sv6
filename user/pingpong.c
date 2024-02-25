#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        char c;
        read(p[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(p[1], "y", 1);
    }
    else
    {
        write(p[1], "x", 1);
        char c;
        read(p[0], &c, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}