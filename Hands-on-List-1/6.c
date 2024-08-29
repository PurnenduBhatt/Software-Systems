/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 6*/

#include<stdio.h>
#include<unistd.h>
int main()
{
	int buffersize=1024;
	char buffer[buffersize];
	ssize_t readbytes,writebytes;
	while((readbytes=read(STDIN_FILENO,buffer,buffersize))>0)
	{
	
	write(STDIN_FILENO,buffer,readbytes);
	}
	if(readbytes==-1)
	{
		perror("Error reading from STDIN");
		return 1;
	}
	return 0;
	
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 6.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out
My name is Purnendu
My name is Purnendu

*/
