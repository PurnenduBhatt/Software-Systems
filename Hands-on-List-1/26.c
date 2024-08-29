/*Name: Purnendu Bhatt
Roll No-MT2024031
Question 26*/
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