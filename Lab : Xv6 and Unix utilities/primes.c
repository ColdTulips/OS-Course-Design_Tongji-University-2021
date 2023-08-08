#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

void sieve(int read_fd)
{
    int prime;
    if (read(read_fd, &prime, sizeof(prime)) == 0) {
        close(read_fd);
        return;
    }

    printf("prime %d\n", prime);

    int pipe_fds[2];
    pipe(pipe_fds);

    int pid = fork();
    if (pid > 0) {
        close(pipe_fds[READ]);
        
        int num;
        while (read(read_fd, &num, sizeof(num)) != 0) {
            if (num % prime != 0) {
                write(pipe_fds[WRITE], &num, sizeof(num));
            }
        }

        close(read_fd);
        close(pipe_fds[WRITE]);
        wait((int*)0);
    } else if (pid == 0) {
        close(pipe_fds[WRITE]);
        sieve(pipe_fds[READ]);
        close(pipe_fds[READ]);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int pipe_fds[2];
    pipe(pipe_fds);

    int pid = fork();
    if (pid > 0) {
        close(pipe_fds[READ]);

        for (int i = 2; i <= 35; i++) {
            write(pipe_fds[WRITE], &i, sizeof(i));
        }

        close(pipe_fds[WRITE]);
        wait((int*)0);
    } else if (pid == 0) {
        close(pipe_fds[WRITE]);
        sieve(pipe_fds[READ]);
        close(pipe_fds[READ]);
        exit(0);
    }

    exit(0);
}