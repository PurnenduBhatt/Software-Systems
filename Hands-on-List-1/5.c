/*
============================================================================
Name : 5.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to create five new files with infinite loop. Execute the program in the background
and check the file descriptor table at /proc/pid/fd.
Date: 28th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
int main()
{
	while(1)
	{
		creat("file5-1",O_CREAT);
		creat("file5-2",O_CREAT);
		creat("file5-3",O_CREAT);
		creat("file5-4",O_CREAT);
		creat("file5-5",O_CREAT);
	}
}
/*
---x------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 19:11 file5-1*
---x------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 19:11 file5-2*
---x------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 19:11 file5-3*
---x------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 19:11 file5-4*
---x------  1 purnendu-bhatt purnendu-bhatt     0 Aug 27 19:11 file5-5*

*/
