/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 10
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
                int writecount=write(fd,"NOTHINGMOR",10);
                if (writecount == -1)
                {
                printf("Error while writing to the file!");
                return 0;
                }   
                offset = lseek(fd, 10, SEEK_SET);//moves file decriptor by 10 bytes
                writecount=write(fd,"SOMUCHMORE",10);
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
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cat 7-1.txt
NOTHINGMORSOMUCHMOREI am pursuing masters at IIIT-Banglroe`*/
