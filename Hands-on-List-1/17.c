/*
Name: Purnendu Bhatt
Roll No-MT2024031
Question 17
*/



#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
    char * ticketfile="ticketfile.txt";
    int fd=open(ticketfile,O_CREAT|O_RDWR,S_IRWXU);
    int buffer=0;
    if(fd==-1)
    {
        printf("Error while opening the file");
    }
    struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_pid = getpid();
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        lock.l_start = 0;
        int fcntlstatus=fcntl(fd,F_GETLK,&lock);
          if (fcntlstatus == -1)
            printf("Error while getting lock status!");
        else
        {
            if(lock.l_type==F_RDLCK)
            printf("file already locked for reading");
            else if(lock.l_type==F_WRLCK)
            printf("file already locked for writing");
            else
            {
               lock.l_type = F_WRLCK;
               fcntl(fd,F_SETLKW,&lock);
               int readbytes=read(fd,&buffer,sizeof(int));
               if(readbytes==-1)
               {
                 printf("Error while reading the file!\n");
                 return 0;
               }
               if(readbytes==0)
               {
                buffer=1;
                write(fd,&buffer,sizeof(int));
                printf("Your ticket number is: %d\n", buffer);
               }
               else
               {
                lseek(fd,0,SEEK_SET);
                read(fd,&buffer,sizeof(int));
                buffer+=1;
                write(fd,&buffer,sizeof(int));
                 printf("Your ticket number is: %d\n", buffer);
               }
               printf("Press Enter to complete the transaction");
               char c=getchar();
               if(c)
               {
                    lock.l_type = F_UNLCK;
                    int fcntlStatus = fcntl(fd, F_SETLKW, &lock);
                    if (fcntlStatus == -1)
                        perror("Error releasing lock!");
                    else
                        printf("Done!\n"); 
               }
                                   
            }
        }
        close(fd);

}
/*


purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ cc 17.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware$ ./a.out
Your ticket number is: 2
Press Enter to complete the transaction
Done!


*/
