/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 13
*/

#include<sys/time.h>//For select
#include<stdio.h>
#include<unistd.h>
int main()
{
    fd_set fd1,fd2;
    FD_SET(STDIN_FILENO,&fd1);
    FD_SET(STDOUT_FILENO,&fd2);
    struct timeval time;
    time.tv_sec=10;
    time.tv_usec=0;
    int returnvalue=select(1,&fd1,&fd2,NULL,&time);
     if (returnvalue == -1)
        perror("ERROR ");
    else if (returnvalue) // returnVal -> 1 due because STDIN will contain the data
        printf("The data is now available\n");
    else // returnVal -> 0 due to time out
        printf("No data was given for 10 seconds\n");
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 13.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
No data was given for 10 seconds

*/