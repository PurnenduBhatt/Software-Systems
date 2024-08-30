/*
============================================================================
Name : 25.c
Author : Purnendu Bhatt
Description : Write a program to create three child processes. The parent should wait for a particular child (use
waitpid system call).
Date: 28th Aug, 2024.
============================================================================
*/
#include<stdio.h>
#include <sys/wait.h>
#include<unistd.h>
int main()
{
    int childa,childb,childc,terminatedchild,waitstatus;
      if ((childa = fork()) == 0)
    {
        // Child A will enter this branch
        printf("Child A created...\n");
        printf("Putting child A to sleep for 5 sec\n");
        sleep(5);
        printf("Child A is now awake!\n");
        _exit(0);
    }
    else
    {
        // Parent will enter this branch
        if ((childb = fork()) == 0)
        {
            // Child B will enter this branch
            printf("Child B created...\n");
            printf("Putting child B to sleep for 10 sec\n");
            sleep(10);
            printf("Child B is now awake!\n");
            _exit(0);
        }
        else
        {
            // Parent will enter this branch

            if ((childc = fork()) == 0)
            {
                // Child C will enter this branch
                printf("Child C created...\n");
                printf("Putting child C to sleep for 15 sec\n");
                sleep(15);
                printf("Child C is now awake!\n");
                _exit(0);
            }
            else
            {
                terminatedchild=waitpid(childc,&waitstatus,0);
                 if (terminatedchild == childc)
                {
                    if (waitstatus == 0)
                        printf("Child C has terminated with exit status = 0\n");
                    else
                        printf("Child has exited with exit status : %d\n", waitstatus);
                }
            }
        }
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 25.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out
Child A created...
Putting child A to sleep for 5 sec
Child B created...
Putting child B to sleep for 10 sec
Child C created...
Putting child C to sleep for 15 sec
Child A is now awake!
Child B is now awake!
Child C is now awake!
Child C has terminated with exit status = 0

*/
