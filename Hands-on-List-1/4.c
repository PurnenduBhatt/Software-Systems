/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 4*/


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
