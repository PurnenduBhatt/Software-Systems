/*
============================================================================
Name : 16b.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style to perform mandatory locking.
b. Implement read lock
Date: 28th Aug, 2024.
============================================================================
*/
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char*argv[])
{
    struct flock lock,lockstatus;
    if(argc!=2)
    {
        printf("Pass the file name as the parameter");
        return 0;
    }
    int fd=open(argv[1],O_RDWR|O_CREAT);
        lock.l_type = F_RDLCK;    // Write lock
        lock.l_whence = SEEK_SET; // Seek to start of the file
        lock.l_start = 0;         // Lock from start of the file
        lock.l_len = 0;           // Locks the entire file
        lock.l_pid = getpid();
        lockstatus=lock;
        fcntl(fd,F_GETLK,&lockstatus);
        if(lockstatus.l_type==F_WRLCK)
        {
            printf("File already locked for writing");
        }
        else
        {
            fcntl(fd,F_SETLK,&lock);
             printf("File is now locked for reading");
        }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 16b.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out file16.txt
File is now locked for reading
*/
