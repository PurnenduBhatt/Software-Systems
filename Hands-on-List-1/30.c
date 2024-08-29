/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 30
*/
#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
int main()
{
    if(!fork())
    {
        setsid();
        chdir("/");
        umask(0);
        while(1)
        {
            sleep(4);
            printf("child process=%d\n",getpid());
            printf("Daemon process is running\n");
        }
    }
    else
    exit(0);
}
/*

purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 30.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ 
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ child process=3373
Daemon process is running

purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ child process=3373
Daemon process is running
child process=3373
Daemon process is running
child process=3373
Daemon process is running

*/
