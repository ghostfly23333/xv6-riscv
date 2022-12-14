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
    int i; 
    int pid;

    for(i=0;i<3;i++)
        pid=fork();

    for(i=0;i<10000;i++)
        var_add();

    printf("%d :%d\n",pid,var_read());

    sleep(10);
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