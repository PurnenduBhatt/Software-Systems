
/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write two programs: first program is waiting to catch SIGSTOP signal, the second program
    will send the signal (using kill system call). Find out whether the first program is able to catch
    the signal or not.
*/
#include <sys/types.h> 
#include <signal.h>    
#include <unistd.h>    
#include <stdio.h>     
#include <stdlib.h> 

void main(int argc, char *argv[])
{
    int killStatus; 
    pid_t pid;

    if (argc != 2)
    {
        printf("Pass the PID of the process to whom the SIGSTOP signal is to be sent!\n");
        _exit(0);
    }

    pid = atoi(argv[1]);

    killStatus = kill(pid, SIGSTOP);

    if(!killStatus) 
        printf("Successfully sent SIGSTOP signal to process (%d)\n", pid);
    else 
        perror("Error while sending signal!");
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 13b.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out 9603
Successfully sent SIGSTOP signal to process (9603)

*/