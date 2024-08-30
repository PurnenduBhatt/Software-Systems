/*
============================================================================
Name : 21.c
Author : Purnendu Bhatt
Description : Write a program, call fork and print the parent and child process id.
Date: 28th Aug, 2024.
============================================================================
*/
#include<unistd.h>
#include<stdio.h>
int main()
{
    int parentid=getpid();
    printf("Parent process id=%d\n",parentid);
    int childpid=fork();
    if(childpid!=0)
    {
        printf("Child process id=%d",childpid);
    }
    return 0;
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 21.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out
Parent process id=6554
Child process id=6555
*/
