/*
============================================================================
Name : 20.c
Author : Purnendu Bhatt
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 28th Aug, 2024.
============================================================================
*/
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char * argv[])
{
    int priority,newp;
    if(argc!=2)
    {
        printf("Please enter the new priority to be added");
        return 0;
    }
    newp=atoi(argv[1]);
    priority=nice(0);
    printf("the old priority is %d",priority);
    priority=nice(newp);
    printf("the new priority is %d",priority);
    return 1;
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 20.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out 5
the old priority is 0
the new priority is 5
*/
