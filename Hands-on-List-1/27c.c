/*
============================================================================
Name : 27c.c
Author : Purnendu Bhatt
Description : Write a program to execute ls -Rl by the following system calls
c. execle
Date: 28th Aug, 2024.
============================================================================
*/
#include <unistd.h> 
#include <stdio.h>  

void main()
{
    char *command_path = "/bin/ls";
    char *options = "-Rl";
    char *file_arg = "./samplefiles"; 

    printf("============OUTPUT USING execle===============\n");
    execle(command_path, command_path, options, file_arg, NULL, NULL);
    printf("\n");
}
/*
============OUTPUT USING execle===============
./samplefiles:
total 16
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 95 Aug 28 21:17 7-1.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 91 Aug 28 21:19 7-2.txt
-r-x-wS--T 1 purnendu-bhatt purnendu-bhatt 31 Aug 28 21:21 file11c.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 33 Aug 27 22:34 file22.txt

*/
