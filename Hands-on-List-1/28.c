/*
============================================================================
Name : 28.c
Author : Purnendu Bhatt
Description : Write a program to get maximum and minimum real time priority.
Date: 28th Aug, 2024.
============================================================================
*/

#include<sched.h>
#include<stdio.h>
int main()
{
    int maxpriority,minpriority;
    maxpriority=sched_get_priority_max(SCHED_RR);
     if (maxpriority == -1)
        printf("Error while finding max priority\n");
    else
        printf("The max priority with RR Scheduling Policy is : %d\n", maxpriority);
    minpriority=sched_get_priority_min(SCHED_RR);
    if (minpriority == -1)
        perror("Error while finding min priority\n");
    else
        printf("The min priority with RR Scheduling Policy is : %d\n", minpriority);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 28.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
The max priority with RR Scheduling Policy is : 99
The min priority with RR Scheduling Policy is : 1
*/
