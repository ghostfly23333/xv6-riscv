#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 测试哲学家就餐问题
void test1(int m){
    int i; 

    for(i=0;i<3;i++)
        fork();
    // 如果为0，测试加了同步策略的
    // 如果为1，测试未加同步的
    philosopher(m);
}

// 测试场景1：验证是否实现同步
void test2(){
    
    printf("--access shared variable--\n");
    printf("8 processes add 1 to the shared variable for 10000 times respectively.\n");
    printf("If the implementation is correct, then the final value of the shared variable will be 80000.\n\n");
    
    int i; 
    int mutex_out = sem_create(1);

    for(i=0;i<3;i++)
        fork();

    for(i=0;i<10000;i++)
        var_add();

    sem_wait(mutex_out);
    printf("Process %d exited. The value of the shared variable is %d.\n", getpid(), var_read());          
    sem_signal(mutex_out);
}

int 
main(int argc,char* argv[])
{
    if(argv[1][0]=='a')//add
        test2();
    else if(argv[1][2]=='_')//ph_not
        test1(1);
    else if (argv[1][0]=='p')//ph
        test1(0);
    else if (argv[1][0]=='c')//clear
        var_clear();
    else
        printf("invalid command!\n");

    exit(0);
}