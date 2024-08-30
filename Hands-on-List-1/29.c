/*
============================================================================
Name : 29.c
Author : Purnendu Bhatt
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
Date: 28th Aug, 2024.
============================================================================
*/
#include<sched.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
    int pid;
    pid=getpid();
    int currentpolicy=sched_getscheduler(pid);
    struct sched_param priority;
    priority.sched_priority=10;
    switch(currentpolicy)
    {
        case SCHED_FIFO:
                        printf("Current policy is Fifo\n");
                        sched_setscheduler(pid,SCHED_RR,&priority);
                        currentpolicy=sched_getscheduler(pid);
                        printf("Current policy is %d\n",currentpolicy);
                        break;
        case SCHED_RR:
                        printf("Current policy is RR\n");
                        sched_setscheduler(pid,SCHED_FIFO,&priority);
                        currentpolicy=sched_getscheduler(pid);
                        printf("Current policy is %d\n",currentpolicy);
                        break;

        case SCHED_OTHER:
                        printf("Current policy is OTHER\n");
                        sched_setscheduler(pid,SCHED_RR,&priority);
                        currentpolicy=sched_getscheduler(pid);
                        printf("Current policy is %d\n",currentpolicy);
                        break;
        default:
                        printf("Unable to fetch the current policy\n");
                        break;


        
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 29.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
Current policy is OTHER
Current policy is 0

*/
