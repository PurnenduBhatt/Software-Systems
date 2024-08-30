/*
============================================================================
Name : 1b.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to create the following types of a files using (i) shell command (ii) system call
b. hard link (link system call)
Date: 28th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
int main()
{
	link("1.txt","1bhardlink");
	return 0;
}
/*
-rw-rw-r--  2 purnendu-bhatt purnendu-bhatt     0 Aug 27 18:42 1bhardlink
*/
