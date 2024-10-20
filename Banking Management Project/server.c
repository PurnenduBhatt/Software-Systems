#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>  // Include threading library
#include <ctype.h>
#include "roleselection.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_LENGTH 100

// Function to trim leading and trailing whitespace from a string
void strtrim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate
    *(end + 1) = '\0';
}

// Function to handle client connections
void *handle_client(void *client_socket_ptr) {
    int client_sock = *(int *)client_socket_ptr;
    free(client_socket_ptr);  // Free the memory allocated for the socket pointer

    char username[MAX_LENGTH] = {0}, password[MAX_LENGTH] = {0}, role[MAX_LENGTH] = {0};
    char buffer[BUFFER_SIZE] = {0};

    bzero(username, sizeof(username));
    bzero(password, sizeof(password));
    bzero(role, sizeof(role));
    bzero(buffer, sizeof(buffer));

    usleep(400000);  // Sleep for 400 milliseconds

    // Prompt for role
    write(client_sock, "Enter your role (Customer/Employee/Manager/Administrator): ", 58);
    read(client_sock, role, MAX_LENGTH);
    strtrim(role);

    // Prompt for username and password
    write(client_sock, "Enter username: ", 16);
    int bytes_read = read(client_sock, username, MAX_LENGTH);
    if (bytes_read > 0) {
        username[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    write(client_sock, "Enter password: ", strlen("Enter password: "));
    bytes_read = read(client_sock, password, MAX_LENGTH);
    if (bytes_read > 0) {
        password[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  

    // Login the user
    if (login_user(username, password, role) != -1) {
        create_session(username);  // Create session

        // Call role-specific menu
        if (strcmp(role, "Customer") == 0) {
            customer_menu(client_sock, username);
        } else if (strcmp(role, "Employee") == 0) {
            employee_menu(client_sock, username);
        } else if (strcmp(role, "Manager") == 0) {
            manager_menu(client_sock, username);
        } else if (strcmp(role, "Administrator") == 0) {
            admin_menu(client_sock, username);
        }
    } else {
        close(client_sock);  // Close the client socket
        pthread_exit(NULL);   // Terminate the thread
    }

    close(client_sock);  // Close the client socket after handling
    pthread_exit(NULL);  // Terminate the thread
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up address and port for server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        if ((client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted.\n");

        // Allocate memory for client socket pointer (for passing to thread)
        int *client_socket_ptr = malloc(sizeof(int));
        if (client_socket_ptr == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        *client_socket_ptr = client_sock;

        // Create a new thread for each client connection
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void *)client_socket_ptr) != 0) {
            perror("Thread creation failed");
            free(client_socket_ptr);
            exit(EXIT_FAILURE);
        }

        // Detach the thread so it cleans up automatically when finished
        pthread_detach(client_thread);
    }

    return 0;
}
