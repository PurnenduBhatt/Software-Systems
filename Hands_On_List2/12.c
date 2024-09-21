/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
    the parent process from the child process.
*/
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
void main()
{
    int childpid=fork();
    if(!childpid)
    {
        printf("%d\n",getpid());
        printf("%d\n",getppid());
        sleep(5);
        int killstatus=kill(getppid(),SIGKILL);
        if(!killstatus)
        printf("Successfully killed parent\n");
    }
    else{
        while(1);
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 12.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
6760
6759
Successfully killed parent
Killed
*/