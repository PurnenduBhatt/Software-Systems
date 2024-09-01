/*
============================================================================
Name : 18a.c
Author : Purnendu Bhatt
Description : Write a C, Ansi-style program to perform Record locking.
                       a. Implement write lock
Date: 28th Aug, 2024.
============================================================================
*/



#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main()
{

    struct {
    int trainno;
    int tickno;
}db;
    int fd=open("record.txt",O_RDWR);
    struct flock lock;
    int train;
    printf("Enter the train no (1,2,3,4)");
    scanf("%d",&train);
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(train-1)*sizeof(db);
    lock.l_pid=getpid();
    lock.l_len=sizeof(db);
    lseek(fd,(train-1)*sizeof(db),SEEK_SET);
    printf("PLease wait \n");
    fcntl(fd,F_SETLKW,&lock);
    read(fd,&db,sizeof(db));
    printf("no of tickets sold is %d\n",db.tickno);
    
    db.tickno++;
    lseek(fd,-1*sizeof(db),SEEK_CUR);
    
    
    lock.l_type=F_UNLCK;
    printf("press Enter to book ticket\n");
    getchar();
    getchar();
    printf("Your ticket no is %d\n",db.tickno);
    write(fd,&db,sizeof(db));
    fcntl(fd,F_SETLKW,&lock);
    
    return 0;
    
    
    


}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ cc 18.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-S$ ./a.out
Enter the train no (1,2,3,4)3
PLease wait 
no of tickets sold is 9
press Enter to book ticket

Your ticket no is 10

*/
