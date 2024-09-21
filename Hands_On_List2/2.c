
/*
Name:Purnendu Bhatt
Roll No:MT2024031
Program Description:Write a program to print the system resource limits. Use getrlimit system call.
Date:12 Sep 2024
*/

#include<sys/resource.h>
#include<stdio.h>
void print(char *s,rlim_t soft,rlim_t hard, char* unit)
{
    printf("%s:-\n",s);
    printf("Soft Limit->");
    if(soft==RLIM_INFINITY)
    {
        printf("UNLIMITED\n");
    }
    else{
        printf("%ld %s\n",soft,unit);
    }
    printf("Hard Limit->");
    if(soft==RLIM_INFINITY)
    {
        printf("UNLIMITED\n");
    }
    else{
        printf("%ld %s\n",hard,unit);
    }
    printf("==========================================================================================================\n");
}
int main()
{
    struct rlimit resourcelimit;
    int status=getrlimit(RLIMIT_AS,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_AS");
    }
    else
    {
        print("Max Size of Process Virtual Memory",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    status=getrlimit(RLIMIT_CORE,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_CORE");
    }
    else
    {
        print("Max size of a core file",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    status=getrlimit(RLIMIT_CPU,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_CPU");
    }
    else
    {
        print("Max CPU time that the process can consume",resourcelimit.rlim_cur,resourcelimit.rlim_max,"seconds");
    }
    status=getrlimit(RLIMIT_DATA,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_DATA");
    }
    else
    {
        print("Max size of process's data segement",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    status=getrlimit(RLIMIT_FSIZE,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_FSIZE");
    }
    else
    {
        print("Max size of files that  the  process  may  create",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_LOCKS,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_LOCKS");
    }
    else
    {
        print("Max number of locks that a process may establish",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_MEMLOCK,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_MEMLOCK");
    }
    else
    {
        print("Max number of bytes of memory that may be locked into RAM",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    status=getrlimit(RLIMIT_MSGQUEUE,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_MSGQUEUE");
    }
    else
    {
        print("Max number of bytes of that can be allocated for POSIX message queues",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    status=getrlimit(RLIMIT_NICE,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_NICE");
    }
    else
    {
        print("Ceiling of the process's nice value",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_NOFILE,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_NOFILE");
    }
    else
    {
        print("Max file descriptor",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_NPROC,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_NPROC");
    }
    else
    {
        print("Limit on the number of threads",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_RSS,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_RSS");
    }
    else
    {
        print("Limit on the  number of virtual pages resident in the RAM",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    
    status=getrlimit(RLIMIT_RTPRIO,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_RTPRIO");
    }
    else
    {
        print("Ceiling on the real-time priority",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_RTTIME,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_RTTIME");
    }
    else
    {
        print("Amount of CPU time that a  process scheduled  under a real-time scheduling policy",resourcelimit.rlim_cur,resourcelimit.rlim_max,"seconds");
    }
    status=getrlimit(RLIMIT_SIGPENDING,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_SIGPENDING");
    }
    else
    {
        print("The number of signals that may be queued",resourcelimit.rlim_cur,resourcelimit.rlim_max,"");
    }
    status=getrlimit(RLIMIT_STACK,&resourcelimit);
    if(status==-1)
    {
        perror("Error while gettimg RLIMIT_STACK");
    }
    else
    {
        print("Maximum  size of the process stack",resourcelimit.rlim_cur,resourcelimit.rlim_max,"Bytes");
    }
    
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 2.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Max Size of Process Virtual Memory:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Max size of a core file:-
Soft Limit->0 Bytes
Hard Limit->-1 Bytes
==========================================================================================================
Max CPU time that the process can consume:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Max size of process's data segement:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Max size of files that  the  process  may  create:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Max number of locks that a process may establish:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Max number of bytes of memory that may be locked into RAM:-
Soft Limit->1009504256 Bytes
Hard Limit->1009504256 Bytes
==========================================================================================================
Max number of bytes of that can be allocated for POSIX message queues:-
Soft Limit->819200 Bytes
Hard Limit->819200 Bytes
==========================================================================================================
Ceiling of the process's nice value:-
Soft Limit->0 
Hard Limit->0 
==========================================================================================================
Max file descriptor:-
Soft Limit->1024 
Hard Limit->1048576 
==========================================================================================================
Limit on the number of threads:-
Soft Limit->30515 
Hard Limit->30515 
==========================================================================================================
Limit on the  number of virtual pages resident in the RAM:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
Ceiling on the real-time priority:-
Soft Limit->0 
Hard Limit->0 
==========================================================================================================
Amount of CPU time that a  process scheduled  under a real-time scheduling policy:-
Soft Limit->UNLIMITED
Hard Limit->UNLIMITED
==========================================================================================================
The number of signals that may be queued:-
Soft Limit->30515 
Hard Limit->30515 
==========================================================================================================
Maximum  size of the process stack:-
Soft Limit->8388608 Bytes
Hard Limit->-1 Bytes
==========================================================================================================


*/