/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 12
*/


#include <fcntl.h>     
#include <unistd.h>
#include <stdio.h>

void main(int argc, char *argv[])
{
    char *filename;     // File name of the source file
    int fileDescriptor; // File descriptor of the source file
    int status;         // Holds the result of the fcntl call. -1 -> Error

    if (argc != 2)
        printf("Pass the file to be opened as the argument!\n");
    else
    {
        filename = argv[1];

        fileDescriptor = open(filename, O_WRONLY);

        if (fileDescriptor == -1)
            perror("Error while opening the file");
        else
        {
            status = fcntl(fileDescriptor, F_GETFL);

            if (status == -1)
                perror("Error on executing fcntl!");
            else
            {
                
                switch (O_ACCMODE & status)
                {
                case 0:
                    printf("The file has been opened with the flags : O_RDONLY\n");
                    break;
                case 1:
                    printf("The file has been opened with the flags : O_WRONLY\n");
                    break;
                case 2:
                    printf("The file has been opened with the flags : O_RDWR\n");
                    break;
                case 64:
                    printf("The file has been opened with the flags : O_CREAT\n");
                    break;
                case 512:
                    printf("The file has been opened with the flags : O_TRUNC\n");
                    break;
                case 1024:
                    printf("The file has been opened with the flags : O_APPEND\n");
                    break;
                case 577:
                    printf("The file has been opened with the flags : O_WRONLY | O_CREAT | O_TRUNC\n");
                    break;
                case 1089:
                    printf("The file has been opened with the flags : O_WRONLY | O_CREAT | O_APPEND\n");
                    break;
                case 578:
                    printf("The file has been opened with the flags : O_RDWR   | O_CREAT | O_TRUNC\n");
                    break;
                case 1090:
                    printf("The file has been opened with the flags : O_RDWR   | O_CREAT | O_APPEND\n");
                    break;
                default:
                    printf("Error!");
                }
            }
            close(fileDescriptor);
        }
    }
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 12.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt
The file has been opened with the flags : O_WRONLY
*/