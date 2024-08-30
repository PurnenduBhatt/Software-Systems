/*
============================================================================
Name : 7.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to copy file1 into file2 ($cp file1 file2).
Date: 28th Aug, 2024.
============================================================================
*/
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
int main(int argc,char*argv[])
{
	char*source;
	char*destination;
	source=argv[1];
	destination = argv[2];
	int fd1=open(source,O_RDONLY);
	if(fd1==-1)
	{
		printf("error while opening the file");
	}
	int fd2=open(destination,O_CREAT|O_RDWR,S_IRWXU);
	int readcount;
	int writecount;
	char buffer[100];
	while((readcount=read(fd1,buffer,100))>0)
	{
		writecount=write(fd2,buffer,readcount);
		if(writecount==-1)
		{
			printf("Error while printing the file");
		}
	}
	if(readcount==-1)
	{
		printf("Error while reading the file");
	}
	close(fd1);
	close(fd2);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 7.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ vim 7-1.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt 7-2.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat 7-2.txt
My name is purnendu
I am pursuing masters at IIIT-Banglroe`

*/
