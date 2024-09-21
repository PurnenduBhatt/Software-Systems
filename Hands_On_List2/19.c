/*
    Name:Purnendu Bhatt
    Roll No:MT2024031
    Program Description:Create a FIFO file by
    a. mknod command
    b. mkfifo command
    c. use strace command to find out, which command (mknod or mkfifo) is better.
    c. mknod system call
    d. mkfifo library function
*/
#include <sys/types.h> // Import for `mkfifo` library function & `mknod` system call
#include <sys/stat.h>  // Import for `mkfifo` library function & `mknod` system call
#include <fcntl.h>     // Import for `mknod` system call
#include <unistd.h>    // Import for `mknod` system call
#include <stdio.h>     // Import for using `printf` & `perror` function

void main()
{
    char *mkfifoName = "./mymkfifo";    // File name of FIFO file created using `mkfifo`
    char *mknodName = "./mymknod-fifo"; // File name of FIFO file created using `mknod`

    int mkfifo_status, mknod_status; // 0 -> Success, -1 -> Error

    // Using `mkfifo` library function
    mkfifo_status = mkfifo(mkfifoName, S_IRWXU);

    if (mkfifo_status == -1)
        perror("Error while creating FIFO file!");
    else
        printf("Succesfully created FIFO file. Check using `ll` or `ls -l` command!\n");

    // Using `mknod` system call
    mkfifo_status = mknod(mknodName, __S_IFIFO | S_IRWXU, 0);

    if (mknod_status == -1)
        perror("Error while creating FIFO file!");
    else
        printf("Succesfully created FIFO file. Check using `ll` or `ls -l` command!\n");
}
/*
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ cc 19.c
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ./a.out
Succesfully created FIFO file. Check using `ll` or `ls -l` command!
Succesfully created FIFO file. Check using `ll` or `ls -l` command!
purnendu-bhatt@purnendu-bhatt-Inspiron-3501:~/systemsoftware/Software-Systems/Hands-On-List-II$ ll
total 164
drwxrwxr-x 2 purnendu-bhatt purnendu-bhatt  4096 Sep 21 10:14 ./
drwxrwxr-x 5 purnendu-bhatt purnendu-bhatt  4096 Sep 11 04:48 ../
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   907 Sep 19 15:26 10a.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   872 Sep 19 15:27 10b.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   886 Sep 19 15:33 10c.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1566 Sep 19 15:42 11.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   838 Sep 19 15:56 12.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1050 Sep 19 19:15 13a.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1114 Sep 19 19:16 13b.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1318 Sep 20 09:50 14.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1479 Sep 20 10:18 15.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  2076 Sep 20 19:04 16.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1144 Sep 20 19:30 17a.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1177 Sep 20 19:33 17b.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1235 Sep 20 19:35 17c.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  2738 Sep 20 19:43 18.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1437 Sep 21 10:14 19.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1261 Sep 18 15:52 1a.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1270 Sep 12 14:47 1b.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1262 Sep 12 14:52 1c.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  8423 Sep 12 16:26 2.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1214 Sep 12 11:20 3.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   782 Sep 18 14:11 4.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  2859 Sep 18 14:53 5.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   868 Sep 18 14:54 6.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   826 Sep 18 14:55 7.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   788 Sep 18 15:55 8a.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   782 Sep 18 16:16 8b.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   785 Sep 18 16:20 8c.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt   796 Sep 18 16:29 8d.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1266 Sep 18 17:54 8e.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1274 Sep 18 18:00 8f.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1239 Sep 18 18:04 8g.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1177 Sep 18 18:59 9-1.c
-rw-rw-r-- 1 purnendu-bhatt purnendu-bhatt  1008 Sep 19 15:24 9.c
-rwxrwxr-x 1 purnendu-bhatt purnendu-bhatt 16088 Sep 21 10:14 a.out*
prwx------ 1 purnendu-bhatt purnendu-bhatt     0 Sep 21 10:14 mymkfifo|

*/