#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if(argc != 1) printf("dont input arguments\n");

    int parent_to_child[2];
    int child_to_parent[2];
    char buffer[8];

    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1)
    {
        printf("Pipe creation failed\n");
        exit(1);
    }

    int pid = fork();

    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }

    if (pid == 0) // Child process
    {
        close(parent_to_child[WRITE_END]);
        read(parent_to_child[READ_END], buffer, sizeof(buffer));
        close(parent_to_child[READ_END]);

        printf("%d: received %s", getpid(), buffer);

        close(child_to_parent[READ_END]);
        write(child_to_parent[WRITE_END], "pong\n", 5);
        close(child_to_parent[WRITE_END]);

        exit(0);
    }
    else // Parent process
    {
        close(parent_to_child[READ_END]);
        write(parent_to_child[WRITE_END], "ping\n", 5);
        close(parent_to_child[WRITE_END]);

        close(child_to_parent[WRITE_END]);
        read(child_to_parent[READ_END], buffer, sizeof(buffer));
        close(child_to_parent[READ_END]);

        printf("%d: received %s", getpid(), buffer);

        wait((int *)0);

        exit(0);
    }
}