/*
============================================================================
Name : 15.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to display the environmental variable of the user (use environ).
Date: 28th Aug, 2024.
============================================================================
*/
#include <stdio.h> // Import for `NULL`, `environ`

extern char **environ;

int isUser(char *var)
{
    return var[0] == 'U' && var[1] == 'S' && var[4] == '=';
}

void main()
{
    int iter = -1;
    while (environ[++iter] != NULL)
        if (isUser(environ[iter]))
            printf("%s\n", environ[iter]);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 15.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
USER=purnendu-bhatt

*/
