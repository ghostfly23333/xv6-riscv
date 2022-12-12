#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int id = 1;
    id = sem_create(1);
    printf("%d\n", id);
    
    int pid = fork(); 
    int status, i;   

    for(i = 0; i < 10000; i++)
    {   
        sem_wait(id);
        sh_var_write();
        sem_signal(id);
    }
       
    if(pid > 0)
    {
        wait(&status);   
        sem_free(id);
        printf("pid%d: result = %d\n", pid, sh_var_read());
    }   
    else
    {
        printf("pid%d: result = %d\n", pid, sh_var_read());
    }

    exit(0);
}