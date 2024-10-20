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
struct Employee employee[10000]; // Array to store employee data
int usercnt = 0;  // Counter for the number of users

// Function to trim spaces from the beginning and end of a string
void strtrim1(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate
    *(end + 1) = '\0';
}

// Function to load users based on role (Customer, Manager, Administrator)
void load_users(const char *role) {
    FILE* fd;
    
    // Open the appropriate file based on the role
    if (strcmp(role, "Customer") == 0) {
        fd = fopen("users.txt", "r");
    } else if (strcmp(role, "Manager") == 0) {
        fd = fopen("manager.txt", "r");
    } else if (strcmp(role, "Administrator") == 0) {
        fd = fopen("admin.txt", "r");
    } else {
        fd = fopen("employees.txt", "r");  // Default to employees.txt if the role is not matched
    }

    if (fd == NULL) {
        perror("INVALID FILE");
        exit(0);
    }

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  // Clear buffer
    usleep(100000);  // Small delay

    // Load data based on the role
    if (strcmp(role, "Customer") == 0) {
        while (fgets(buffer, sizeof(buffer), fd)) {
            buffer[strcspn(buffer, "\n")] = 0;  // Strip newline character
            sscanf(buffer, "%[^,],%[^,],%d,%s", users[usercnt].username, users[usercnt].password, &users[usercnt].balance, users[usercnt].status);
            printf("Loaded User: %s %s\n", users[usercnt].username, users[usercnt].password);  // Debugging output
            usercnt++;
        }
    } else {  // Load data for Manager, Administrator, or employees from employees.txt
        while (fgets(buffer, sizeof(buffer), fd)) {
            buffer[strcspn(buffer, "\n")] = 0;  // Strip newline character
            sscanf(buffer, "%[^,],%[^,],%s", employee[usercnt].username, employee[usercnt].password, employee[usercnt].role);
            printf("Loaded Employee: %s %s\n", employee[usercnt].username, employee[usercnt].password);  // Debugging output
            usercnt++;
        }
    }

    fclose(fd);
    printf("Number of users loaded: %d\n", usercnt);  // Debugging to check user count
}

// Function to hash the password
void hash_password(const char *password, char *hashed_password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, strlen(password), hash);

    // Convert the hash to a hexadecimal string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[64] = '\0';  // Null-terminate the string
}

// Function to verify login credentials
int verify_login(char *username, char *password, char *role) {
    char hashed_password[65];  // Buffer to store the hashed password
    strtrim1(username);  // Ensure username is trimmed
    strtrim1(password);  // Ensure password is trimmed

    username[strcspn(username, "\n")] = '\0';  // Remove trailing newline
    password[strcspn(password, "\n")] = '\0';  // Remove trailing newline

    hash_password(password, hashed_password);  // Hash the input password

    printf("Input Username: %s\n", username);  // Debugging output
    printf("Hashed Password for input: %s\n", hashed_password);

    for (int i = 0; i < usercnt; i++) {
        if (strcmp(role, "Customer") == 0) {
            printf("Stored Username: %s\n", users[i].username);
            printf("Stored Hashed Password: %s\n", users[i].password);

            strtrim1(users[i].username);  // Trim the username
            strtrim1(users[i].password);  // Trim the password
            
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, hashed_password) == 0) {
                printf("Login successful for user: %s\n", users[i].username);
                return i;  // Return the index if a match is found
            }
        } else if (strcmp(role, "Manager") == 0 || strcmp(role, "Administrator") == 0 ||strcmp(role, "Employee") == 0) {
            printf("Stored Employee Username: %s\n", employee[i].username);
            printf("Stored Hashed Password: %s\n", employee[i].password);

            strtrim1(employee[i].username);  // Trim the username
            strtrim1(employee[i].password);  // Trim the password

            if (strcmp(employee[i].username, username) == 0 && strcmp(employee[i].password, hashed_password) == 0) {
                printf("Login successful for employee: %s\n", employee[i].username);
                return i;  // Return the index if a match is found
            }
        }
    }

    return -1;  // Return -1 if no match is found
}