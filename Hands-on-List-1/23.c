
/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 23
*/
#include<unistd.h>
#include<stdio.h>
int main()
{
    int id;
    id=fork();
    if(id!=0)
    {
        printf("parent=%d, Parent is asleep now for 50 seconds\n",getpid());
        sleep(50);
        printf("Parent is awake now \n");
    }
    else
    {
        printf("child=%d",getpid());
        printf("Exiting child");

    }
    return 0;
}
/*
top - 23:12:58 up 11 min,  2 users,  load average: 1.29, 0.92, 0.47
Tasks: 237 total,   1 running, 235 sleeping,   0 stopped,   1 zombie
%Cpu(s): 13.8 us,  2.3 sy,  0.0 ni, 83.8 id,  0.0 wa,  0.0 hi,  0.2 si,  0.0 st 
MiB Mem :   7701.9 total,   3675.3 free,   2586.7 used,   2355.5 buff/cache     
MiB Swap:   4096.0 total,   4096.0 free,      0.0 used.   5115.2 avail Mem 
*/