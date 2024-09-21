/*
Name:Purnendu Bhatt
Roll No:MT2024031
Program Description:Write a program to measure how much time is taken to execute 100 getppid ( )
system call. Use time stamp counter.
Date:12 Sep 2024
*/

#include<unistd.h>
#include<stdio.h>
#include <x86intrin.h>
void main(){

    int start=__rdtsc();
    int itr=0;
    while(itr<100)
    {
        int pid=getpid();
        itr++;

    }
    int end=__rdtsc();
    printf("Time needed to execute 100 getpid() system calls is %f ns",(end-start)/2.4);
    
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 4.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Time needed to execute 100 getpid() system calls is 13162.083333 ns
*/