#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void prime(int *p) {
  int n;
  close(p[1]);
  if (read(p[0], &n, sizeof(n)) < sizeof(n)) {
    close(p[0]);
    return;
  }
  fprintf(1, "prime %d\n", n);
  int q[2];
  if (pipe(q) < 0) {
    fprintf(1, "pipe failed\n");
    exit(0);
  }
  if (fork() == 0) {
    close(p[0]);
    prime(q);
  } else {
    close(q[0]);
    int i;
    while (read(p[0], &i, sizeof(i)) == sizeof(i)) {
      if (i % n != 0) {
        write(q[1], &i, sizeof(i));
      }
    }
    close(p[0]);
    close(q[1]);
    wait((int*)0);
  }
}

int main(int argc, char *argv[]) {
  int p[2];
  if (pipe(p) < 0) {
    fprintf(1, "pipe failed\n");
    exit(1);
  }
  if (fork() == 0) {
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]); 
    exit(0);
  } else {
    close(p[1]);
    prime(p);
    wait((int*)0);
  }
  exit(0);
}