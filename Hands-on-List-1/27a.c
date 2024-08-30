/*
============================================================================
Name : 27a.c
Author : Purnendu Bhatt
Description : Write a program to execute ls -Rl by the following system calls
a. execl
Date: 28th Aug, 2024.
============================================================================
*/

#include<unistd.h>
#include<stdio.h>

int main()
{
    char * commandpath="/bin/ls";
    char * options="-Rl";
    char *filearg="./samplefiles";
    printf("Output using execl ");
    execl(commandpath,commandpath,options,filearg,NULL);
    printf("\n");
}
/*purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 27a.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
./samplefiles:
total 16
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 95 Aug 28 21:17 7-1.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 91 Aug 28 21:19 7-2.txt
-r-x-wS--T 1 purnendu-bhatt purnendu-bhatt 31 Aug 28 21:21 file11c.txt
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt 33 Aug 27 22:34 file22.txt*/

