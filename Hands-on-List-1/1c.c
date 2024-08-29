/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 1c*/


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
