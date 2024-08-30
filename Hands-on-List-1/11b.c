/*
============================================================================
Name : 11b.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not..
b. use dup2
Date: 28th Aug, 2024.
============================================================================
*/
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
    

    if(argc!=2)
    {
        printf("Pass filename as parameter");

    }
    else{
        int fd=open(argv[1],O_WRONLY|O_CREAT|O_APPEND);
        if(fd==-1)
        {printf("Problem while opening the file");
        return 0;
        }
        int dupfd=dup2(fd,156);
        if(dupfd==-1)
        {
            printf("Problem while duplicating the file");
            return 0;
        }
        int writecount=write(dupfd,"Using duplicate File descriptor",31);
        if(writecount==-1)
        {
            printf("Problem while writing the file");
        }
        return 0;

    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 11b.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-2.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat 7-2.txt
My name is purnendu
I am pursuing masters at IIIT-Banglroe`
Using duplicate File descriptor
*/
