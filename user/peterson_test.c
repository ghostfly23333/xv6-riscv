#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    printf("--access shared variable--\n");
    printf("Two processes add 1 to the shared variable for 1000 times respectively.\n");
    printf("If the implementation is correct, then the final value of the shared variable will be 2000.\n\n");
    
    int i, status;
    int pid =  fork();
    for(i = 0; i < 1000; i++)
    {
        if(pid > 0) procedure0();
        else procedure1();       
    }
      
    if(pid > 0) wait(&status);       
    printf("Process %d exited. The value of the shared variable is %d.\n", pid, read_cnt()); 
    exit(0);
}