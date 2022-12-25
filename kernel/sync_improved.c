#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#define SEM_MAX_NUM 128

struct sem
{
  struct spinlock lock;   
  int resource_count;
  int allocated;
};

struct sem sems[SEM_MAX_NUM];
int sh_var = 0;

uint64 
sys_sh_var_read()
{
  return sh_var;
}

uint64 
sys_sh_var_write()
{
  int n;
  argint(0, &n);
  sh_var = n;
  return sh_var;
}

void
initsem(void)
{
  int i;
  for(i = 0; i < SEM_MAX_NUM; i++)
  {
    initlock(&(sems[i].lock), "semaphore");
    sems[i].allocated = 0;
  }
}

int
sys_sem_create(void)
{
  int n_sem, i;
  argint(0, &n_sem);
  // 扫描sem数组，找到第一个未被分配的sem
  for(i = 0; i < SEM_MAX_NUM; i++)
  {
    acquire(&sems[i].lock);
    if(sems[i].allocated == 0)
    {
      sems[i].allocated = 1;
      sems[i].resource_count = n_sem;
      // printf("create sem %d\n", i);
      release(&sems[i].lock);
      return i;
    }
    release(&sems[i].lock);
  } 
  // 未找到
  return -1;
}

int
sys_sem_free(void)
{
  int id;
  argint(0, &id);
  acquire(&sems[id].lock);
  if(sems[id].allocated == 1 && sems[id].resource_count > 0)
  {
    sems[id].allocated = 0;
    // printf("free sem %d\n", id);
  }
  release(&sems[id].lock);
  return 0;
}

int 
sys_sem_wait(void)
{
  int id;
  argint(0, &id);
  acquire(&sems[id].lock);
  sems[id].resource_count--;
  if(sems[id].resource_count < 0)
    sleep(&sems[id], &sems[id].lock);
  release(&sems[id].lock);
  return 0;
}

int 
sys_sem_signal(void)
{
  int id;
  argint(0, &id);
  acquire(&sems[id].lock);
  sems[id].resource_count++;
  if(sems[id].resource_count <= 0)
    wakeup_1pro(&sems[id]);
  release(&sems[id].lock);
  return 0;
}

