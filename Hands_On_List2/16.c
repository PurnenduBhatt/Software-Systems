/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description: Write a program to send and receive data from parent to child vice versa. Use two way
    communication.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int pipe1[2];  // Pipe 1: Parent to Child
    int pipe2[2];  // Pipe 2: Child to Parent
    __pid_t pid;
    char parent_msg[] = "Hello from Parent!";
    char child_msg[] = "Hello from Child!";
    char read_msg[100];

    // First pipe (Parent to Child)
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(1);
    }

    // Second pipe (Child to Parent)
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(1);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    // Child process
    if (pid == 0) {
        // Closeing unnecessary ends of the pipes
        close(pipe1[1]);  // write end of pipe1 (Parent to Child) in the child
        close(pipe2[0]);  // read end of pipe2 (Child to Parent) in the child

        // Read from pipe1 (Parent to Child)
        read(pipe1[0], read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        // Write to pipe2 (Child to Parent)
        write(pipe2[1], child_msg, strlen(child_msg) + 1);

        close(pipe1[0]);  
        close(pipe2[1]);  
    }
    // Parent process
    else {
        close(pipe1[0]);
        close(pipe2[1]);

        // Write to pipe1 (Parent to Child)
        write(pipe1[1], parent_msg, strlen(parent_msg) + 1);

        // Read from pipe2 (Child to Parent)
        read(pipe2[0], read_msg, sizeof(read_msg));
        printf("Parent received: %s\n", read_msg);

        close(pipe1[1]);  
        close(pipe2[0]); 
    }

    return 0;
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 16.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Child received: Hello from Parent!
Parent received: Hello from Child!

*/