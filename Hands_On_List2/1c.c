/*
Name:Purnendu Bhatt
Roll No:MT2024031
Program Description:Write a separate program (for each time domain) to set a interval timer in 10sec and
10micro second
c. ITIMER_PROF
Date:12 Sep 2024
*/

#include<signal.h>
#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
void handler(int signal)
{
    printf("Program will now terminate");
    exit(0);
}

void main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Pass 1 for 10s timer and 2 for 10ms timer");
        exit(0);
    }
    signal(SIGPROF,handler);
    struct itimerval timer; 
    
    if((int)(*argv[1]-48)==1)
    {
        timer.it_interval.tv_sec=0;
        timer.it_interval.tv_usec=0;
        timer.it_value.tv_sec=10;
        timer.it_value.tv_usec=0;
    }
    else if((int)(*argv[1]-48)==2)
    {
        timer.it_interval.tv_sec=0;
        timer.it_interval.tv_usec=0;
        timer.it_value.tv_sec=0;
        timer.it_value.tv_usec=10000;
    }
        int timerStatus = setitimer(ITIMER_PROF, &timer, 0);
        while(1);
}
/*

 purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 1c.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out 1
Program will now terminate
*/