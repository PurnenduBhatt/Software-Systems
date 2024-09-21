/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write a simple program to create a pipe, write to the pipe, read from pipe and display on
    the monitor.
*/
#include <unistd.h> 
#include <stdio.h>  

void main()
{
    
    int fd[2];                
    char *writeBuffer = "Yolo!!"; 
    char *readBuffer;
    int pipeStatus;            
    int readBytes, writeBytes; 
    pipeStatus = pipe(fd);

    if (pipeStatus == -1)
        perror("Error while creating the pipe! ");
    else
    {
        printf("Writing to the pipe!\n");
        writeBytes = write(fd[1], &writeBuffer, sizeof(writeBuffer));
        if (writeBytes == -1)
            perror("Error while writing to pipe!");
        else
        {
            printf("Now reading from the pipe!\n");
            readBytes = read(fd[0], &readBuffer, writeBytes);
            if (readBytes == -1)
                perror("Error while reading from pipe!");
            else
                printf("Data from pipe: %s\n", readBuffer);
        }
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 14.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Writing to the pipe!
Now reading from the pipe!
Data from pipe: Yolo!!
*/