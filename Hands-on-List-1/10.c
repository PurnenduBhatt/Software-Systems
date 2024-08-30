/*
============================================================================
Name : 10.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date: 28th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>//for open
#include<fcntl.h>//for write
int main(int argc,char *argv[])
{
    
if(argc!=2)
{
        printf("Pass the file name as the parameter");
}
else
{
        int offset;
        int fd=open(argv[1],O_RDWR);
        if(fd==-1)
        {
                printf("Error while opening the file");
        }
        else
        {
                int writecount=write(fd,"NOTHINGMOR",11);
                if (writecount == -1)
                {
                printf("Error while writing to the file! ");
                return 0;
                }   
                offset = lseek(fd, 10, SEEK_SET);//moves file decriptor by 10 bytes
                writecount=write(fd,"SOMUCHMORE ",11);
                if (writecount == -1)
                {
                printf("Error while writing to the file!");
                return 0;
                } 

        }
        close(fd);
}
return 0;

}/*purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 10.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out 7-1.txt
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ od -c 7-1.txt
0000000   N   O   T   H   I   N   G   M   O   R   S   O   M   U   C   H
0000020   M   O   R   E   I       a   m       p   u   r   s   u   i   n
0000040   g       m   a   s   t   e   r   s       a   t       I   I   I
0000060   T   -   B   a   n   g   l   r   o   e   `  \n   U   s   i   n
0000100   g       o   r   i   g   i   n   a   l       F   D   U   s   i
0000120   n   g       d   u   p   l   i   c   a   t   e       F   D
0000137
*/
