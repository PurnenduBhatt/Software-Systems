/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Write a program to create a message queue and print the key and message queue id.
*/
#include <sys/types.h> // Import for `ftok` & `msgget`
#include <sys/ipc.h>   // Import for `ftok` & `msgget`
#include <sys/msg.h>   // Import for `msgget`
#include <stdio.h>     // Import for `perror` & `printf`
#include <unistd.h>    // Import for `_exit`
#include <errno.h>     // Import for `errno`

void main()
{
    key_t queueKey;      // IPC (Message Queue) key
    int queueIdentifier; // IPC (Message Queue) identifier

    queueKey = ftok(".", 1);

    if (queueKey == -1)
    {
        perror("Error while computing key!");
        _exit(0);
    }

    queueIdentifier = msgget(queueKey, IPC_CREAT | IPC_EXCL | 0700);
    if (queueIdentifier == -1)
    {
        perror("Error while creating Message Queue!");
        _exit(0);
    }

    printf("Key: %d\n", queueKey);
    printf("Message Queue Identifier: %d\n", queueIdentifier);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 24.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Key: 18181047
Message Queue Identifier: 0
*/