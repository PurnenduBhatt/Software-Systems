/*
============================================================================
Name : 4.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 28th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
int main()
{
      int fd=open("file3",O_EXCL,0644);
      if(fd==-1)
      {
	      printf("Error while opening the file");

      }
      else
      {
	      printf("the fd value of opened file is %d",fd);
      }
      return 0;

	
}
/*
the fd value of opened file is 3
*/
