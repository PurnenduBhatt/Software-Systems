/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 5*/

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
