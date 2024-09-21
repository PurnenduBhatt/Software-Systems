/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -
    use sigaction system call.
*/
#include <signal.h> 
#include <stdio.h>  
#include <unistd.h> 

void signalHandler(int signalNumber)
{
    printf("Caught signal SIGINT (%d)\n", signalNumber);
}

void main()
{
    int status; 
    struct sigaction action;

    printf("Ignoring SIGINT for 5 seconds!\n");
    action.sa_handler = SIG_IGN;
    action.sa_flags = 0;
    status = sigaction(SIGINT, &action, NULL);
    if (status == -1)
        perror("Error while setting signal handler!");
    else
        sleep(5);


    printf("Handling SIGINT for 5 seconds!\n");
    action.sa_handler = signalHandler;
    action.sa_flags = 0;
    status = sigaction(SIGINT, &action, NULL);
    if (status == -1)
        perror("Error while setting signal handler!");
    else
        sleep(5);

    printf("Default action for SIGINT for 5 seconds!\n");
    action.sa_handler = SIG_DFL;
    action.sa_flags = 0;
    status = sigaction(SIGINT, &action, NULL);
    if (status == -1)
        perror("Error while setting signal handler!");
    else
        sleep(5);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 11.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Ignoring SIGINT for 5 seconds!
^CHandling SIGINT for 5 seconds!
^CCaught signal SIGINT (2)
Default action for SIGINT for 5 seconds!
^C
*/