/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write two programs: first program is waiting to catch SIGSTOP signal, the second program
    will send the signal (using kill system call). Find out whether the first program is able to catch
    the signal or not.
*/
#include <signal.h>
#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h> 

void customHandler()
{
    printf("Received SIGSTOP signal!\n");
    _exit(0);
}

void main()
{
    __sighandler_t signalStatus; 
    pid_t pid = getpid();

    printf("Process ID: %d\n", pid);

    signalStatus = signal(SIGSTOP, customHandler);
    sleep(15);
    if (signalStatus == SIG_ERR)
        perror("Error while catching signal!");

    printf("Putting the process to sleep for 15s\n");
    
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 13a.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Process ID: 9603

[1]+  Stopped                 ./a.out
*/
