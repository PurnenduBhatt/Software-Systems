/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 22
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