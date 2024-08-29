/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 24
*/
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    pid_t childPid;

    if ((childPid = fork()) != 0)
    {
        printf("Parent PID: %d\n", getpid());
        printf("Putting Parent to sleep for 10s\n");
        sleep(10); 
        printf("Exiting parent!\n");
        _exit(0);
    }
    else
    {
        printf("Child PID: %d\n", getpid());
        printf("Putting child to sleep for 100s!\n");
        sleep(100); 
        printf("Child is now awake!\n");
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 24.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
Parent PID: 8318
Putting Parent to sleep for 10s
Child PID: 8319
Putting child to sleep for 100s!
Exiting parent!
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ Child is now awake!


*/
