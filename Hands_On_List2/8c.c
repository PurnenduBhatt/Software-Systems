/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write a separate program using signal system call to catch the following signals.
    c. SIGFPE
    Date:12 Sep 2024
*/
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void func()
{
    printf("Signal is catched");
    exit(0);
}
int main()
{
    __sighandler_t signalstatus;
    signalstatus=signal(SIGFPE,(void*)func);
    if(signalstatus==SIG_ERR)
    {
        perror("Error while assigning Signal Handler");

    }
    else
        raise(SIGFPE);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 8c.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Signal is catched
*/