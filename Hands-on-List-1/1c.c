/*
============================================================================
Name : 1c.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to create the following types of a files using (i) shell command (ii) system call
c. FIFO (mkfifo Library Function or mknod system call)
Date: 28th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	int mkfifostatus=mkfifo("mkfifofile",0777);
	if(mkfifostatus==-1)
	{
		printf("Mkfifo file not created");
	}
	else
	{
		printf("mkfifo file created");
	}
	int mknodstatus=mknod("mknode",__S_IFIFO|S_IRWXU,0);
	if(mknodstatus==0)
	{
		printf("Mknodfile created");
	}
	else
	{
		printf("mknod file not created");
	}

	return 0;
}
/*
prwxrwxr-x  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 18:51 mkfifofile|
prwx------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 18:51 mknode|

*/
