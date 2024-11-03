#include "UserManagementModule.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>


struct User users[10000];
struct Employee employee[10000]; 
int usercnt = 0;  


void strtrim1(char *str) {
    char *end;

    
    while (isspace((unsigned char)*str)) str++;

    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    
    *(end + 1) = '\0';
}


void load_users(const char *role) {
    FILE* fd;
    
    
    if (strcmp(role, "Customer") == 0) {
        fd = fopen("users.txt", "r");
    } else if (strcmp(role, "Manager") == 0) {
        fd = fopen("manager.txt", "r");
    } else if (strcmp(role, "Administrator") == 0) {
        fd = fopen("admin.txt", "r");
    } else {
        fd = fopen("employees.txt", "r");  
    }

    if (fd == NULL) {
        perror("INVALID FILE");
        exit(0);
    }

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  
    usleep(100000);  

    
    if (strcmp(role, "Customer") == 0) {
        while (fgets(buffer, sizeof(buffer), fd)) {
            buffer[strcspn(buffer, "\n")] = 0;  
            sscanf(buffer, "%[^,],%[^,],%d,%s", users[usercnt].username, users[usercnt].password, &users[usercnt].balance, users[usercnt].status);
            printf("Loaded User: %s %s\n", users[usercnt].username, users[usercnt].password);  
            usercnt++;
        }
    } else {  
        while (fgets(buffer, sizeof(buffer), fd)) {
            buffer[strcspn(buffer, "\n")] = 0;  
            sscanf(buffer, "%[^,],%[^,],%s", employee[usercnt].username, employee[usercnt].password, employee[usercnt].role);
            printf("Loaded Employee: %s %s\n", employee[usercnt].username, employee[usercnt].password);  
            usercnt++;
        }
    }

    fclose(fd);
    printf("Number of users loaded: %d\n", usercnt);  
}


void hash_password(const char *password, char *hashed_password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, strlen(password), hash);

    
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[64] = '\0';  
}


int verify_login(char *username, char *password, char *role) {
    char hashed_password[65];  
    strtrim1(username);  
    strtrim1(password);  

    username[strcspn(username, "\n")] = '\0';  
    password[strcspn(password, "\n")] = '\0';  

    hash_password(password, hashed_password);  

    printf("Input Username: %s\n", username);  
    printf("Hashed Password for input: %s\n", hashed_password);

    for (int i = 0; i < usercnt; i++) {
        if (strcmp(role, "Customer") == 0) {
            printf("Stored Username: %s\n", users[i].username);
            printf("Stored Hashed Password: %s\n", users[i].password);

            strtrim1(users[i].username);  
            strtrim1(users[i].password);  
            
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, hashed_password) == 0) {
                printf("Login successful for user: %s\n", users[i].username);
                return i;  
            }
        } else if (strcmp(role, "Manager") == 0 || strcmp(role, "Administrator") == 0 ||strcmp(role, "Employee") == 0) {
            printf("Stored Employee Username: %s\n", employee[i].username);
            printf("Stored Hashed Password: %s\n", employee[i].password);

            strtrim1(employee[i].username);  
            strtrim1(employee[i].password);  

            if (strcmp(employee[i].username, username) == 0 && strcmp(employee[i].password, hashed_password) == 0) {
                printf("Login successful for employee: %s\n", employee[i].username);
                return i;  
            }
        }
    }

    return -1;  
}