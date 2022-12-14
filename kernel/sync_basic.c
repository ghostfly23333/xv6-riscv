#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h" 

struct semphore {
    int val;
    struct spinlock lk;
};
struct semphore var_lock;
struct semphore chopstick[8];
struct semphore ph_num;
int reader_count = 0;
int var=0;

void init_semphores()
{
    initlock(&(var_lock.lk), "var_lock");
    var_lock.val = 1;

    for(int i=0;i<8;i++)
    {
        initlock(&(chopstick[i].lk), "chopstick");
        chopstick[i].val = 1;
    }

    initlock(&(ph_num.lk), "ph_num");
    ph_num.val = 7;
}

void WAIT(struct semphore *s) {
    while(1) {
        acquire(&s->lk);
        if(s->val > 0) {
            s->val = s->val - 1;
            release(&s->lk);
            break;
        }
        release(&s->lk);
    }
}

void SIGNAL(struct semphore *s) {
    acquire(&s->lk);
    s->val = s->val + 1;
    release(&s->lk);   
}

uint64 sys_var_read(void) 
{  
    WAIT(&var_lock);
    int temp=var;
    SIGNAL(&var_lock);
    return temp;
}

uint64 sys_var_add(void) 
{
    WAIT(&var_lock);
    var++;
    SIGNAL(&var_lock);
    return 0;
}

uint64 philosopher_not(){
    int i=sys_var_read();
    sys_var_add();
    for(int j=0;j<10;j++)
    {
        if(chopstick[i].val==1&&chopstick[(i+1)%8].val==1)
        {
            // 拿起筷子
            chopstick[i].val=0;
            chopstick[(i+1)%8].val=0;
            printf("Philosopher %d is eating\n",i);
            // 如果说自己还没放下筷子，同时有其他人放下了筷子说明
            // 发生了冲突
            if(chopstick[i].val==1||chopstick[(i+1)%8].val==1)
            {
                printf("error!!\n\n");                          
            }
        }
        // 放下筷子
        chopstick[i].val=1;
        chopstick[(i+1)%8].val=1;

        printf("Philosopher %d finished eating\n",i);        
    }
    return 0;   
}

uint64 philosopher(){
    int i=sys_var_read();
    sys_var_add();
    while (1)
    {
        // 先获得上桌子这一计数信号量
        WAIT(&ph_num);
         // 拿筷子
        WAIT(&(chopstick[i]));
        WAIT(&(chopstick[(i+1)%8]));

        printf("Philosopher %d is eating\n",i);
        if(chopstick[i].val==1||chopstick[(i+1)%8].val==1)          
            printf("error!!\n\n");    

        // 放下筷子                                  
        SIGNAL(&(chopstick[(i+1)%8]));
        SIGNAL(&(chopstick[i]));
        SIGNAL(&ph_num);
        printf("Philosopher %d finished eating\n",i);
    }
}

uint64 sys_philosopher(void)
{
    int n;
    argint(0,&n);
    if(n==0)
        philosopher();
    else
        philosopher_not();
    return 0;
}

uint64 sys_var_clear(void)
{
    WAIT(&var_lock);
    var=0;
    SIGNAL(&var_lock);
    return 0;
}