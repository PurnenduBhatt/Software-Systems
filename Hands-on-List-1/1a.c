/*
============================================================================
Name : 1a.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to create the following types of a files using (i) shell command (ii) system call
a. soft link (symlink system call)
Date: 28th Aug, 2024.
============================================================================
*/


#include<stdio.h>
#include<unistd.h>
int main()
{
	symlink("1.txt","1asoftlink1");
	return 0;  
}

/* output:-
lrwxrwxrwx  1  purnendu-bhatt purnendu-bhatt     5 Aug 27 18:46 1asoftlink1 -> 1.txt
 
*/
