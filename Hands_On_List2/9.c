/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description: Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
    signal - Use signal system call.
*/

#include<signal.h>
#include<stdio.h>
#include<unistd.h>
void func()
{
    printf("Signal has been caught\n");
}
void main()
{
    __sighandler_t signalstatus;
    printf("Your Signal will be ignored for 5 seconds\n");
    signalstatus=signal(SIGINT,SIG_IGN);
    sleep(5);
    signalstatus=signal(SIGINT,(void*)func);
    sleep(5);
    printf("Now signal will perform according to default behaviour\n");
    signalstatus=signal(SIGINT,SIG_DFL);
    sleep(5);



}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 9.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Your Signal will be ignored for 5 seconds
^C^C^CSignal has been caught
Now signal will perform according to default behaviour
^C

*/
