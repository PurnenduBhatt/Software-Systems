#include"FileManagementmodule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int check_and_initialize_system(const char * role) {
    struct stat buffer;
    if(strcmp(role,"Customer")==0)
   { if (stat("users.txt", &buffer) != 0) {
        return -1;
    } else {
        load_users(role); 
        return 0;
    }}
    else if(strcmp(role,"Employee")==0)
    {
        if (stat("employees.txt", &buffer) != 0) {
        
        return -1;
    } else {
        load_users(role); 
        return 0;
    }
    }
    else if(strcmp(role,"Manager")==0)
    {
        if (stat("manager.txt", &buffer) != 0) {
        
        return -1;
    } else {
        load_users(role); 
        return 0;
    }
    }
    else if(strcmp(role,"Administrator")==0)
   { if (stat("admin.txt", &buffer) != 0) {
        initialize_system();
        return -1;
    } else {
        load_users(role); 
        return 0;
    }}
}

void initialize_system() {
    char username[MAX_LENGTH], password[MAX_LENGTH], hashed_password[65];

    printf("Initializing system: Creating first Administrator\n");
    printf("Enter Admin Username: ");
    fgets(username, MAX_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;  
    printf("Enter Admin Password: ");
    fgets(password, MAX_LENGTH, stdin);
 
    strtrim1(password);
    strtrim1(username);
    hash_password(password, hashed_password);

    // Open the file for writing; create if it doesn't exist
    int fd = open("admin.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening user file");
        return;
    }

    // Prepare the buffer for writing, using commas as separators
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "%s,%s,Administrator\n", username, hashed_password);
    
    // Write the buffer to the file
    write(fd, buffer, strlen(buffer));
    close(fd);

    printf("First Administrator created successfully.\n");
}

