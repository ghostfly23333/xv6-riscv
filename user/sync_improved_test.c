#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void access_sh_var()
{
    int id = sem_create(1);

    int pid = fork(); 
    int status, i;   

    for(i = 0; i < 10000; i++)
    {   
        sem_wait(id);
        sh_var_write(sh_var_read() + 1);
        sem_signal(id);
    }
   
    if(pid > 0)
    {
        wait(&status);   
        sem_free(id);
    }   
    printf("pid%d: result = %d\n", pid, sh_var_read());
}

void philosophers()
{
    int i, chopsticks[8], allocated[8] = {0};

    for(i = 0; i < 8; i++)
    {
        chopsticks[i] = sem_create(1);
    }
    int num = sem_create(7);
    int mutex_id = sem_create(1);
    int mutex_out = sem_create(1);
    
    for(i = 0; i < 3; i++)
    {
        fork();
    }
    sem_wait(mutex_id);
    int id = sh_var_read();
    sem_wait(mutex_out);
    printf("Philosopher %d came\n", id);
    sem_signal(mutex_out);
    sh_var_write(sh_var_read() + 1);
    sem_signal(mutex_id);

    while(1)
    {
        sem_wait(num);
        sem_wait(chopsticks[id]);
        allocated[id] = 1;
        sem_wait(chopsticks[(id + 1) % 8]);
        allocated[(id + 1) % 8] = 1;
        sem_wait(mutex_out);
        printf("Philosopher %d is eating.\n", id);
        sem_signal(mutex_out);
        if(allocated[id] == 0 || allocated[(id + 1) % 8] == 0) 
            printf("error!\n");
        sem_signal(chopsticks[id]);
        allocated[id] = 0;
        sem_signal(chopsticks[(id + 1) % 8]);
        allocated[(id + 1) % 8] = 0;
        sem_signal(num);
        sem_wait(mutex_out);
        printf("Philosopher %d finished eating.\n", id);
        sem_signal(mutex_out);
    }
}

int main(int argc, char *argv[])
{    
    sh_var_write(0);
    if(argv[1][0] == '1')
        access_sh_var();
    else if(argv[1][0] == '2')
        philosophers();
    else printf("invalid command!\n");

    exit(0);
}
