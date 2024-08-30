/*
============================================================================
Name : 27e.c
Author : Purnendu Bhatt
Description : Write a program to execute ls -Rl by the following system calls
e. execvp
Date: 28th Aug, 2024.
============================================================================
*/

#include <unistd.h> 
#include <stdio.h>  

void main()
{
    
    char *args[] = {"/bin/ls", "-Rl", "./sample-files", NULL}; 
    printf("============OUTPUT USING execvp===============\n");
    execvp(args[0], args);
    printf("\n");
}
/*
============OUTPUT USING execvp===============
./samplefiles:
total 16
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 95 Aug 28 21:17 7-1.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 91 Aug 28 21:19 7-2.txt
-r-x-wS--T 1 purnendu-bhatt purnendu-bhatt 31 Aug 28 21:21 file11c.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 33 Aug 27 22:34 file22.txt
*/