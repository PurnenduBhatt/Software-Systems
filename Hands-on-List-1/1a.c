/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 1a*/


#include<stdio.h>
#include<unistd.h>
int main()
{
	symlink("1.txt","1asoftlink1");
	return 0;  
}

/* output:-
lrwxrwxrwx  1  purnendu-bhatt purnendu-bhatt     5 Aug 27 18:46 1asoftlink1 -> 1.txt
 
*/
