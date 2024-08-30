/*
============================================================================
Name : 26.c
Author : Purnendu Bhatt
Description : Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 28th Aug, 2024.
============================================================================
*/
#include<unistd.h>
int main()
{
    char * commandpath="./samplefiles/14";
    char *arg="14.c";
    execl(commandpath,commandpath,arg,NULL);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 26.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
Regular File

*/
