#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void access_sh_var_not()
{
    printf("--access shared variable--\n");
    printf("2 processes add 1 to the shared variable for 10000 times respectively.\n");
    printf("If the implementation is correct, then the final value of the shared variable will be 20000.\n\n");
    
    int pid = fork(); 
    int status, i;   

    for(i = 0; i < 10000; i++)
    {   
        sh_var_write(sh_var_read() + 1);
    }
   
    if(pid > 0)
    {
        wait(&status);   
    }  
    printf("Process %d exited. The value of the shared variable is %d.\n", getpid(), sh_var_read());
}

void access_sh_var()
{
    printf("--access shared variable--\n");
    printf("2 processes add 1 to the shared variable for 10000 times respectively.\n");
    printf("If the implementation is correct, then the final value of the shared variable will be 20000.\n\n");
    
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
    printf("Process %d exited. The value of the shared variable is %d.\n", getpid(), sh_var_read());    
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
    // sem_wait(mutex_out);
    // printf("Philosopher %d came\n", id);
    // sem_signal(mutex_out);
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
        if(allocated[id] == 0)
        {
            printf("\nerror: chopstick %d is used by 2 philosophers at the same time\n\n", id);
        }
        if(allocated[(id + 1) % 8] == 0)
        {
            printf("\nerror: chopstick %d is used by 2 philosophers at the same time\n\n", (id + 1) % 8);
        }
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
    
    if(argv[1][0] == '0')
        access_sh_var_not();
    else if(argv[1][0] == '1')
        access_sh_var();
    else if(argv[1][0] == '2')
        philosophers();
    else printf("invalid command!\n");

    exit(0);
}
