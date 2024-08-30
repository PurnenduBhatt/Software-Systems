/*
============================================================================
Name : 22.c
Author : Purnendu Bhatt
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file.
Date: 28th Aug, 2024.
============================================================================
*/
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
    int fd;
    fd=open("file22.txt",O_CREAT|O_RDWR|O_APPEND,0666);
    int id=fork();
    if(id==0)
    {
        write(fd,"By child process",16);
    }
    else
    {
        write(fd,"By parent process",17);
    }
    close(fd);
    return 0;
}
/*By parent processBy child process*/
