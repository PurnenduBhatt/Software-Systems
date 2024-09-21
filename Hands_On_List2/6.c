/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description: Write a simple program to create three threads.
    Date:12 Sep 2024
*/
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
void *samplefunction(void *data)
{
    printf("running in thread:%lu\n",pthread_self());
}
void main()
{
    pthread_t threadone,threadtwo,threadthree;
    pthread_create(&threadone,NULL,samplefunction,NULL);
    pthread_create(&threadtwo,NULL,samplefunction,NULL);
    pthread_create(&threadthree,NULL,samplefunction,NULL);
    pthread_exit(NULL);
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 6.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
running in thread:139656691709632
running in thread:139656683316928
running in thread:139656674924224
*/