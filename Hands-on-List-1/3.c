/*
============================================================================
Name : 3.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 28th Aug, 2024.
============================================================================
*/


#include<stdio.h>
#include<fcntl.h>
int main()
{

	int fd=creat("file3",S_IRWXU);
	if(fd==-1)
	{
		printf("Error while creating the file");
	}
	else
	{
		printf("the fd value is %d",fd);
	}
	return 0;


}
/*
the fd value is 3
*/
