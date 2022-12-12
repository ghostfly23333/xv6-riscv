#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int id = sem_create(1);
    int pid = fork(); 
    int status, i;   

    for(i = 0; i < 100000; i++)
    {   
        sem_wait(id);
        sh_var_write();
        sem_signal(id);
    }
    
    printf("pid%d: result = %d\n", pid, sh_var_read());
    if(pid > 0)
    {
        wait(&status);   
        sem_free(id);
    }   

    exit(0);
}