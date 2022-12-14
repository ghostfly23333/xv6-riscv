#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int i, status;
    int pid =  fork();
    for(i = 0; i < 1000; i++)
    {
        if(pid > 0) procedure0();
        else procedure1();       
    }
    if(pid > 0) wait(&status);  
    printf("pid%d: result = %d\n", pid, read_cnt());
            
    exit(0);
}