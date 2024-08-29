
/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 27d
*/
#include <unistd.h> 
#include <stdio.h>  

void main()
{
   
    char *args[] = {"/bin/ls", "-Rl", "./samplefiles", NULL}; 
    printf("============OUTPUT USING execv===============\n");
    execv(args[0], args);
    printf("\n");
}
/*
============OUTPUT USING execv===============
./samplefiles:
total 16
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 95 Aug 28 21:17 7-1.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 91 Aug 28 21:19 7-2.txt
-r-x-wS--T 1 purnendu-bhatt purnendu-bhatt 31 Aug 28 21:21 file11c.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 33 Aug 27 22:34 file22.txt
*/