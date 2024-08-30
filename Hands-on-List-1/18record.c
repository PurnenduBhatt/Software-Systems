
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
struct 
{
    int trainno;
    int tickno;
}db[4];
for(int i=0;i<4;i++)
{
    db[i].trainno=i+1;
    db[i].tickno=0;
}
int fd=open("record.txt",O_CREAT|O_RDWR,0666);
write(fd,db,sizeof(db));
}

