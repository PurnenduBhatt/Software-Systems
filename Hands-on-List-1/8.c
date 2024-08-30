/*
============================================================================
Name : 8.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to open a file in read only mode, read line by line and display each line as it is read.
Close the file when end of file is reached.
Date: 28th Aug, 2024.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
const char EOL = '\n';

int main(){
   FILE* fd = fopen("7-1.txt","r");
   if(fd == NULL){
	perror("INVALID FILE");
	exit(0);
   }
	int c = fgetc(fd);
	char buffer[1024];
	int k = 0;
	while(c!=EOF){
	 while(c!=EOL){
	  buffer[k] = c;
	  k++;
	  c = fgetc(fd);
  	 }
	
	 buffer[k++] = '\n';
	 write(1,buffer,k);
	 buffer[0] = '\0';
	 k=0;
	 c = fgetc(fd);
	}
	fclose(fd);
	return 0;  
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 7.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ vim 7-1.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt 7-2.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat 7-2.txt
My name is purnendu
I am pursuing masters at IIIT-Banglroe`

*/
