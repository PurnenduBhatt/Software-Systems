/*
============================================================================
Name : 11c.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not..
c. use fcntl
Date: 28th Aug, 2024.
============================================================================
*/
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
     if(argc!=2)
    {
        printf("Pass filename as parameter");

    }
    else
    {
        int fd=open(argv[1],O_WRONLY|O_CREAT|O_APPEND);
        if(fd==-1)
        {printf("Problem while opening the file");
        return 0;
        }
        int dupfd=fcntl(fd,F_DUPFD);
        if(dupfd==-1)
        {printf("Problem while duplicating the file");
        return 0;
        }
        int writecount = write(dupfd,"Using duplicate File descriptor",31);
        if(writecount==-1)
        {
            printf("Problem while writing the file");
        }
       
        return 0;
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 11c.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out file11c.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat file11c.txt
Using duplicate File descriptor
*/
