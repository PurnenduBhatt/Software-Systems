/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 21
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