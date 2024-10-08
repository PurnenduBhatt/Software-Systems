/*
============================================================================
Name : 9.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date: 28th Aug, 2024.
============================================================================
*/

#include<sys/stat.h>
#include<stdio.h>
#include<time.h>
int main()
{
	int status;
	struct stat statbuff;
	status=stat("7-1.txt",&statbuff);
	if(status==-1)
	{
		printf("Error while executing the file");
	}
	else
	{
		printf("INODE= %ld\n ",statbuff.st_ino);
		printf("NO of hardlinks=%ld\n",statbuff.st_nlink);
		printf("Uid=%d\n",statbuff.st_uid);
		printf("gid=%d\n",statbuff.st_gid);
		printf("size=%ld\n",statbuff.st_size);
		printf("Blocksize=%ld\n",statbuff.st_blksize);
		printf("No of blocks=%ld\n",statbuff.st_blocks);
		printf("time of last accsess=%s\n",ctime(&statbuff.st_atim.tv_sec));
		printf("time of last modification=%s\n",ctime(&statbuff.st_mtim.tv_sec));
		printf("time of last change=%s\n",ctime(&statbuff.st_ctim.tv_sec));
	}

}
/*
INODE= 4198095
 NO of hardlinks=1
Uid=1000
gid=1000
size=60
Blocksize=4096
No of blocks=8
time of last accsess=Wed Aug 28 21:10:50 2024

time of last modification=Wed Aug 28 21:09:39 2024

time of last change=Wed Aug 28 21:09:39 2024

*/


