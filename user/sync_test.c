#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    fork();
    int i, n;
    for(i = 0; i < 100000; i++)
    {
        n = sh_var_read();
        sh_var_write(n + 1);
    }
    printf("result: %d\n", sh_var_read());
    exit(0);
}