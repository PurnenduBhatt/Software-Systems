/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write two programs so that both can communicate by FIFO -Use two way communications.
*/
#include <sys/types.h> // Import for & `open`
#include <sys/stat.h>  // Import for & `open`
#include <fcntl.h>     // Import for `open`
#include <unistd.h>    // Import for `write`
#include <stdio.h>     // Import for `perror`

#include "./21global.h" // Import for FIFO file name & `init` function

void main()
{
    int readBytes, writeBytes;
    int fifoOneFD, fifoTwoFD;
    char data[100];

    init();

    fifoOneFD = open(fifoOneName, O_RDONLY);
    if (fifoOneFD == -1)
    {
        perror("Error while opening fifoOne");
        _exit(0);
    }

    fifoTwoFD = open(fifoTwoName, O_WRONLY);
    if (fifoOneFD == -1)
    {
        perror("Error while opening fifoTwo");
        _exit(0);
    }

    writeBytes = write(fifoTwoFD, "From 21b to 21a", 15);

    if (writeBytes == -1)
        perror("Error while writing to FIFO fifoOne!");

    close(fifoTwoFD);

    readBytes = read(fifoOneFD, &data, 100);

    if (readBytes == -1)
    {
        perror("Error while reading from FIFO fifoOne!");
        _exit(0);
    }

    printf("Data from FIFO: %s\n", data);

    close(fifoOneFD);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 21b.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Data from FIFO: From 21a to 21b
*/