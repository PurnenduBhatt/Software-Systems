#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<ctype.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
void strtrim(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces
        return;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';
}


// Function to read from server and write user input to server
int read_write(int sock, char *buffer) {
    // Clear the buffer before reading
    bzero(buffer, BUFFER_SIZE); // Clear the buffer using bzero
    usleep(100000); // Sleep for 100 milliseconds to allow for processing

    // Read message from the server
    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1); // Leave room for null terminator
    if (bytes_read <= 0) {
        printf("Failed to read from server or server closed the connection.\n");
        return -1; // Indicate failure
    }

    // Null-terminate the buffer and display the server message
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);

    // Check if the server indicates a logout
    if (strstr(buffer, "Logging out...") != NULL) {
        return 1; // Indicate logout
    }

    // Prompt for user input
    printf("Enter your choice:\n");

    // Clear buffer before receiving input
    bzero(buffer, BUFFER_SIZE); // Clear the buffer again
    usleep(100000); // Sleep again to allow for user input readiness
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        perror("Error reading input");
        return -1; // Indicate failure
    }

    // Remove the newline character at the end of the input
    buffer[strcspn(buffer, "\n")] = '\0';

    // Send the user input (menu choice) to the server
    if (write(sock, buffer, strlen(buffer)) <= 0) {
        printf("Failed to send input to server.\n");
        return -1; // Indicate failure
    }

    return 0; // Indicate success
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Set up server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    while (1) {
        // Read the initial prompt for role selection
        int status = read_write(sock, buffer);
        if (status == -1) break; // Break on failure
        if (status == 1) break; // Break on logout

        if (strcmp(buffer, "customer") == 0) {
            // First interaction: Read username
            status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout

            // Second interaction: Read password
            status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout
            
            // Check if the login was successful
            bzero(buffer, BUFFER_SIZE); // Clear the buffer before reading response
            usleep(100000); // Allow time for server response

            // Enter the menu handling loop
            while (1) {
                // Use read_write to handle menu options
                status = read_write(sock, buffer);
                if (status == -1) break; // Break on failure
                if (status == 1) break; // Break on logout

                // Check if the user wants to view balance or transaction history
                if (strcmp(buffer, "1") == 0 || strcmp(buffer, "8") == 0) {
                    // Read the server's response for these actions without prompting for more input
                    bzero(buffer, BUFFER_SIZE); // Clear the buffer before reading response
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';  // Null-terminate buffer
                        printf("Server Response: %s\n", buffer);  // Display balance or transaction history
                    } else {
                        printf("Failed to read from server.\n");
                        break;
                    }
                } else {
                    // For any other option, call read_write once
                     if (strcmp(buffer, "4") == 0 )

                   { status = read_write(sock, buffer);
                    if (status == -1) break; // Break on failure
                    if (status == 1) break; // Break on logout
                    status = read_write(sock, buffer);
                    if (status == -1) break; // Break on failure
                    if (status == 1) break; // Break on logout
                    
                    }
                    else{
                        status = read_write(sock, buffer);
                    if (status == -1) break; // Break on failure
                    if (status == 1) break; // Break on logout
                    
                    }
                }

                // Check for logout condition again after handling menu
                if (strstr(buffer, "Logging out...") != NULL) {
                    break; // Break the menu loop if logging out
                }
            }
        }
        else  if (strcmp(buffer, "Administrator") == 0) {
            // Admin role actions
               status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout

            // Second interaction: Read password
            status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout
            while(1)  {
            // Add New Employee

              status = read_write(sock, buffer);
                if (status == -1) break; // Break on failure
                if (status == 1) break; // Break on logout

            if (strcmp(buffer, "1") == 0) {
                // First interaction: Enter username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Third interaction: Enter role
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               
                                bzero(buffer, BUFFER_SIZE);
                usleep(100000);

            }

            // Modify User Details
            else if (strcmp(buffer, "2") == 0) {
                // First interaction: Enter username to modify
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter new username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Third interaction: Enter new password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Fourth interaction: Enter new role
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
            }

            // Change Password
            else if (strcmp(buffer, "4") == 0) {
                // First interaction: Enter new password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Final read to confirm success
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print success message
                }
            }

            // Manage User Roles
            else if (strcmp(buffer, "3") == 0) {
    // Keep reading until there's no more data to read
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
        
        // Check for read errors
        if (bytes_read <= 0) {
            printf("Connection closed or error occurred\n");
            break;  // Exit the loop if there's no data
        }

        buffer[bytes_read] = '\0';  // Null-terminate the buffer

        // Trim the buffer to remove spaces/newlines
        strtrim(buffer);

        // Check if the trimmed buffer is "stop"
        if (strcmp(buffer, "stop") == 0 || strcmp(buffer, "stop\0") == 0) {
            printf("Finished reading user roles.\n");
            break;  // Exit the loop if "stop" is received
        }

        // Print the received buffer if it's not "stop"
        printf("%s\n", buffer);  // Print user roles
    }

    // After exiting the loop, display the admin menu again
    printf("Returning to the admin menu...\n");
    // Display the admin menu function or logic
   // Make sure to implement this function
}

            }
        }
        else  if (strcmp(buffer, "Employee") == 0) {
            // Admin role actions
               status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout

            // Second interaction: Read password
            status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout
            while(1)  {
            // Add New Employee

              status = read_write(sock, buffer);
                if (status == -1) break; // Break on failure
                if (status == 1) break; // Break on logout

            if (strcmp(buffer, "1") == 0) {
                // First interaction: Enter username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                

               
            }

            // Modify User Details
            if (strcmp(buffer, "2") == 0) {
                // First interaction: Enter username to modify
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter new username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Third interaction: Enter new password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;



             
            }

            // Change Password
            if (strcmp(buffer, "3") == 0) {
                // First interaction: Enter new password
                strtrim(buffer);
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Final read to confirm success
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print success message
                }
            }

            // Manage User Roles
            if (strcmp(buffer, "5") == 0) {
                // Keep reading until there's no more data to read
                
                while (1) {
                    bzero(buffer, BUFFER_SIZE);
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    strtrim(buffer);
                    if (bytes_read <= 0)
                     {
                        break;  // Exit the loop if there is no more data
                    }
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print user roles
                }
            }
             if (strcmp(buffer, "4") == 0) {
                // First interaction: Enter username to modify
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter new username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               

               
            }
        }
    }
    else  if (strcmp(buffer, "Manager") == 0) {
            // Admin role actions
               status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout

            // Second interaction: Read password
            status = read_write(sock, buffer);
            if (status == -1) break; // Break on failure
            if (status == 1) break; // Break on logout
            while(1)  {
            // Add New Employee

              status = read_write(sock, buffer);
                if (status == -1) break; // Break on failure
                if (status == 1) break; // Break on logout

            if (strcmp(buffer, "1") == 0) {
                // First interaction: Enter username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter password
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                


            }

            // Modify User Details
            if (strcmp(buffer, "2") == 0) {
                // First interaction: Enter username to modify
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

            }

            // Change Password
            if (strcmp(buffer, "3") == 0) {
                // First interaction: Enter new password
                strtrim(buffer);
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Final read to confirm success
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print success message
                }
            }

            // Manage User Roles
            if (strcmp(buffer, "5") == 0) {
                // Keep reading until there's no more data to read
                
                while (1) {
                    bzero(buffer, BUFFER_SIZE);
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    strtrim(buffer);
                    if (bytes_read <= 0)
                     {
                        break;  // Exit the loop if there is no more data
                    }
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print user roles
                }
            }
             if (strcmp(buffer, "4") == 0) {
                // First interaction: Enter username to modify
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                // Second interaction: Enter new username
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               

                // Final read to confirm success
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  // Print success message
                }
            }
        }
    }
   
}
}
