/*
============================================================================
Name : 30.c
Author : Purnendu Bhatt
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 28th Aug, 2024.
============================================================================
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
