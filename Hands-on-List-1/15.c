/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 15
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