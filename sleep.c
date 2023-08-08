#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    if(argc < 2) 
    {
        printf("sleep need one argument\n");
        exit(1);
    }
    uint ticks = atoi(argv[1]);
    sleep(ticks);
    printf("(nothing happens for a little while)");
    exit(0);
}
