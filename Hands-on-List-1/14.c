/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 14
*/


#include<sys/stat.h>//for fstat
#include<fcntl.h>//for open
#include<unistd.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("pass the file name as parameter");
        return 0;
    }
    int fd=open(argv[1],O_RDONLY);
    struct stat statbuf;
    fstat(fd,&statbuf);
        if (S_ISREG(statbuf.st_mode))
            write(STDOUT_FILENO, "Regular File\n", 13);

        else if (S_ISDIR(statbuf.st_mode))
            write(STDOUT_FILENO, "Directory File\n", 15);

        else if (S_ISCHR(statbuf.st_mode))
            write(STDOUT_FILENO, "Character File\n", 15);

        else if (S_ISBLK(statbuf.st_mode))
            write(STDOUT_FILENO, "Block File\n", 11);

        else if (S_ISFIFO(statbuf.st_mode))
            write(STDOUT_FILENO, "FIFO File\n", 10);

        else if (S_ISLNK(statbuf.st_mode))
            write(STDOUT_FILENO, "Symbolic Link File\n", 19);

        else if (S_ISSOCK(statbuf.st_mode))
            write(STDOUT_FILENO, "Socket\n", 7);

        else
            write(STDOUT_FILENO, "Error\n", 6);

}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 14.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt
Regular File
*/