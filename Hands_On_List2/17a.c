/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description: Write a program to execute `ls -l | wc`.
    a. use dup
*/

#include <unistd.h>    
#include <sys/types.h> 
#include <stdio.h>     
void main()
{
    
    int pipefd[2]; 
    int pipeStatus;
    pid_t childPid;

    pipeStatus = pipe(pipefd);

    if (pipeStatus == -1)
        perror("Error while creating the file!");
    else
    {
        childPid = fork();

        if (childPid == -1)
            perror("Error while forking child!");
        else if (childPid == 0)
        {
            close(STDIN_FILENO);
            dup(pipefd[0]); 
            close(pipefd[1]);
            execl("/usr/bin/wc", "wc", NULL);
        }
        else
        {
           
            close(STDOUT_FILENO);
            dup(pipefd[1]); 
            close(pipefd[0]);
            execl("/usr/bin/ls", "ls -l", "-l", NULL);
        }
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 17a.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
     31     272    2021
*/