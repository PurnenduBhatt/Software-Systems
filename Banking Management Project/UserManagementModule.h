#ifndef USER_MANAGEMENT_MODULE_H
#define USER_MANAGEMENT_MODULE_H

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<ctype.h>
#define MAX_LENGTH 100
#define USER_COUNT 10000


struct Employee {
    char username[100];
    char password[100];
    char role[100];
    
};
struct User {
    char username[100];
    char password[100];
    int balance;
    char status[10];
    
};
struct Loan{
    int loanid;
    char username[100];
    int amount;
    char status[10];
};


extern struct User users[USER_COUNT];

extern int usercnt;


void load_users(const char* role);
void hash_password(const char *password, char *hashed_password);
int verify_login( char *username, char *password,char*role);
void strtrim1( char *str);
#endif 
