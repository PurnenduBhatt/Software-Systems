#ifndef FILE_MANAGEMENT_MODULE_H
#define FILE_MANAGEMENT_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
 // Include user management header for related functions and structures
#include "UserManagementModule.h"


// Function to check and initialize the system
int check_and_initialize_system(const char *role);

// Function to initialize the system and create the first Administrator
void initialize_system();

#endif // FILE_MANAGEMENT_MODULE_H
