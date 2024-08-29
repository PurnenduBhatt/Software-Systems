/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 11a
*/
#include <unistd.h>    // Import for `dup` system call
#include <fcntl.h>     // Import for `open` system call
#include <stdio.h>     // Import required for `perror` & `perror`

void main(int argc, char *argv[])
{

    char *fname;                        
    int FD, dupFD; 
    int writeByteCount;                    

    if (argc != 2)
        printf("Pass the file name as the argument!");
    else
    {
        fname = argv[1];
        FD = open(fname, O_WRONLY | O_APPEND);
        if (FD == -1)
            perror("Error while opening file");
        else
        {
            dupFD = dup(FD);

            // Writing using original file descriptor
            writeByteCount = write(FD, "Using original FD", 17);
            if (writeByteCount == -1)
                perror("Error while writing file using original FD");

            // Writing using duplicate file descriptor
            writeByteCount = write(dupFD, "Using duplicate FD", 18);
            if (writeByteCount == -1)
                perror("Error while writing file using duplicate FD");

            close(FD);
        }
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 11a.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat 7-1.txt
NOTHINGMORSOMUCHMOREI am pursuing masters at IIIT-Banglroe`
Using original FDUsing duplicate FD
*/