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

int flag[2] = {0};
int turn;
int cnt = 0;

int read_cnt()
{
  return cnt;
}

void procedure0() 
{
  flag[0] = 1;
  turn = 1;
  /*若flag[1]为0，P0就进入临界区；若flag[1]为1，则P0循环等待，只要P1退出临界区，P0即可进入;*/
  while (flag[1] && turn == 1)
  {
    printf("process 0 is waiting.\n"); 
  };
  /*访问临界区*/
  // printf("process 0 is working.\n"); 
  cnt++;
  /*访问临界区完成，procedure0释放出临界区*/
  // printf("process 0 exited\n"); 
  flag[0] = 0; 
}

void procedure1() 
{
  flag[1] = 1;
  turn = 0;
  while (flag[0] && turn == 0)
  {
    printf("process 1 is waiting.\n"); 
  }
  // printf("process 1 is working.\n"); 
  cnt++;
  // printf("process 1 exited\n"); 
  flag[1] = 0; 
}