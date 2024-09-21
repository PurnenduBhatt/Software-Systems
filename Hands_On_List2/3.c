
/*
Name:Purnendu Bhatt
Roll No:MT2024031
Program Description:Write a program to set (any one) system resource limit. Use setrlimit system call.
Date:12 Sep 2024
*/
#include<sys/resource.h>
#include<stdio.h>
void print(char *s,rlim_t soft,char * unit)
{
    printf("%s:-",s);
    if(soft==RLIM_INFINITY)
    printf("INFINITY\n");
    else
    printf("%ld %s\n",soft,unit);
}

void main()
{
struct rlimit resourcelimit;
int status=getrlimit(RLIMIT_CORE,&resourcelimit);
if(status==-1)
{
    perror("Error while fetching the error");
}
else
{
    print("Current size before changing",resourcelimit.rlim_cur,"bytes");
    resourcelimit.rlim_cur=4096;
    setrlimit(RLIMIT_CORE,&resourcelimit);
    status=setrlimit(RLIMIT_CORE,&resourcelimit);
    if(status==-1)
    {
    perror("Error while fetching the error");
    }   
    else
    {
        print("Current size after changing",resourcelimit.rlim_cur,"bytes");
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 3.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Current size before changing:-0 bytes
Current size after changing:-4096 bytes

*/
}