/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 3*/


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
