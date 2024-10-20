#include"roleselection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<ctype.h>
// Function to check if the user is already logged in
// Function to log out the user

static void strtrim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate
    *(end + 1) = '\0';
}
void logout(const char *username) {
    int fd = open("sessions.txt", O_RDWR);
    if (fd < 0) {
        return;  // Session file not found
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        close(fd);
        return;  // Could not acquire lock
    }

    char buffer[MAX_LENGTH];
    int temp_fd = open("temp_sessions.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    while (read(fd, buffer, sizeof(buffer)) > 0) {
        if (!strstr(buffer, username)) {
            write(temp_fd, buffer, strlen(buffer));  // Write unchanged session
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    rename("temp_sessions.txt", "sessions.txt");  // Update the session file
}

// Function to log in a user based on role
int login_user( char *username,  char *password,  char *role) {
    char line[1024];
    int fd;
    
    int x=check_and_initialize_system(role);
    printf("x=%d\n",x);

    if(x==-1)
    return -1;
    x=is_user_logged_in(username,role);
     if(x==1)
    return -1;
printf("x=%d\n",x);
    // Determine which file to check based on role
   
  

    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  
    int verified=verify_login(username,password,role);
    printf("\nverified=%d\n",verified);
    if(verified!=-1)
    {
       
        return 0;
    }

    // Check credentials
    //while (read(fd, line, sizeof(line)) > 0) {
       // char current_username[MAX_LENGTH], current_password[MAX_LENGTH];
        //sscanf(line, "%[^|]|%[^|]|", current_username, current_password);
        //if (strcmp(current_username, username) == 0 && strcmp(current_password, password) == 0) {
         //   lock.l_type = F_UNLCK;  // Unlock
          //  fcntl(fd, F_SETLK, &lock);
          //  close(fd);
          //  create_session(username);  // Create session on successful login
          //  return 0;  // Successful login
        //}
   // }

    
    return -1;  // Failed login
}

// Implement customer_menu
void customer_menu(int sock, const char *username) {
    char buffer[1024];

    while (1) {
        // Send the customer menu to the client
        char menu[]="\nCustomer Menu:\n"
            "1. View Account Balance\n"
            "2. Deposit Money\n"
            "3. Withdraw Money\n"
            "4. Transfer Funds\n"
            "5. Apply for a Loan\n"
            "6. Change Password\n"
            "7. Add Feedback\n"
            "8. View Transaction History\n"
            "9. Logout\n"
            "Enter your choice: ";

        write(sock, menu, strlen(menu));
        printf("Menu sent to client.\n"); // Debugging line

        memset(buffer, 0, sizeof(buffer));  // Clear buffer

        ssize_t bytesRead = read(sock, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            printf("Connection closed by client or error occurred.\n");
            break;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the string

        // Handle the choice received from the client
        int choice = atoi(buffer);
        switch (choice) {
            case 1:
                get_balance(sock, username);
                break;
            case 2:
                depositMoney(sock, username);
                break;
            case 3:
                withdrawMoney(sock, username);
                break;
            case 4:
                transferfunds(sock, username);
                break;
            case 5:
                apply_for_loan(sock, username);
                break;
            case 6:
                change_password(sock, username,"Customer");
                break;
            case 7:
                add_feedback(sock);
                break;
            case 8:
                view_transaction_history(sock, username);
                break;
            case 9:
                logout(username);
                write(sock,"Logging out...",strlen("Logging out..."));
                
            
            default:
                write(sock, "Invalid choice. Please try again.\n", 34);
        }
    }

    close(sock);
}

// Implement customer functions

// Function to modify (deposit/withdraw) balance for a specific user
// `amount` > 0 for deposit, < 0 for withdrawal
int lock_file(int fd, int lock_type, off_t offset, size_t size) {
    printf("INSIDE LOCK FILE %d",fd);
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = lock_type;  // F_RDLCK for reading, F_WRLCK for writing
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;  // Start of the record
    lock.l_len = size;      // Size of the record (size of the structure)

    return fcntl(fd, F_SETLKW, &lock);  // Set the lock, blocking if necessary
}

// Function to unlock the specific record
int unlock_file(int fd, off_t offset, size_t size) {
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_UNLCK;  // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;  // Start of the record
    lock.l_len = size;      // Size of the record (size of the structure)

    return fcntl(fd, F_SETLKW, &lock);  // Unlock the record
}








int modify_balance(int sock, const char *username, int amount) {
    int fd = open("users.txt", O_RDONLY);  // Open the file for reading
    if (fd < 0) {
        return -1;  // Error opening file
    }

    struct User user;
    char buffer[1024];  // Buffer to hold file content
    ssize_t bytes_read;
    size_t total_bytes = 0;

    // Read the entire file content into the buffer
    while ((bytes_read = read(fd, buffer + total_bytes, sizeof(buffer) - total_bytes - 1)) > 0) {
        total_bytes += bytes_read;

        // If the buffer is nearly full, stop reading more
        if (total_bytes >= sizeof(buffer) - 1) {
            break;
        }
    }
    close(fd);  // Close the read file

    buffer[total_bytes] = '\0';  // Null-terminate the buffer

    // Create a temporary file to write updated user data
    int tmp_fd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tmp_fd < 0) {
        return -1;  // Error creating temporary file
    }

    // Tokenize the buffer based on newlines to process each record
    char *record = strtok(buffer, "\n");
    int balance_updated = 0;

    // Process each record
    while (record != NULL) {
        // Clear the user structure
        memset(&user, 0, sizeof(user));  // Clear user structure before using

        // Parse the record manually using sscanf (comma-separated values)
        int parsed = sscanf(record, "%[^,],%[^,],%d,%[^,\n]", user.username, user.password, &user.balance, user.status);
        if (parsed == 4) {  // Ensure that all fields are parsed

            // Check if this is the record we want to update
            if (strcmp(username, user.username) == 0) {
                // Check if the user is inactive
                if (strcmp(user.status, "inactive") == 0) {
                    close(tmp_fd);
                    return -1;  // User is inactive
                }

                // Check if it's a withdrawal and ensure sufficient balance
                if (amount < 0 && user.balance < (-amount)) {
                    close(tmp_fd);
                    return -1;  // Insufficient balance
                }

                // Update balance for the matching user
                user.balance += amount;
                balance_updated = 1;  // Mark that we updated the balance
            }

            // Write the record (updated or not) to the temporary file
            dprintf(tmp_fd, "%s,%s,%d,%s\n", user.username, user.password, user.balance, user.status);
        }

        // Move to the next record
        record = strtok(NULL, "\n");  // Get the next record
    }

    close(tmp_fd);  // Close the temporary file

    // Replace the old file with the new one
    rename("temp.txt", "users.txt");

    return balance_updated ? 0 : -1;  // Success or user not found
}




void get_balance(int sock, const char *username) {
    int fd = open("users.txt", O_RDONLY);  // Open the file for reading
    if (fd < 0) {
        write(sock, "Failed to open file.\n", strlen("Failed to open file.\n"));
        return;
    }

    char buffer[400];
    char line_buffer[1024];  // Buffer to accumulate data for each line
    ssize_t bytes_read;
    size_t line_length = 0;  // Keep track of the length of the accumulated line
    int found = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer for safe string operations

        // Process each character in the buffer
        for (size_t i = 0; i < bytes_read; i++) {
            // Add the current character to the line buffer
            if (line_length < sizeof(line_buffer) - 1) {
                line_buffer[line_length++] = buffer[i];
            }

            // If we encounter a newline, we have a complete line
            if (buffer[i] == '\n') {
                line_buffer[line_length] = '\0';  // Null-terminate the line

                // Process the line (comma-separated values)
                char *token = strtok(line_buffer, ",");
                char *user = token;  // First element (username)
                token = strtok(NULL, ",");  // Second element (ignored)
                token = strtok(NULL, ",");  // Third element (balance)
                char *balance = token;  // Store the balance for the response
                token = strtok(NULL, ",");  // Fourth element (ignored)

                // Check if this is the record we want
                if (user && strcmp(username, user) == 0) {
                    // Send the balance over the socket
                    if (balance) {
                        char message[100];
                        snprintf(message, sizeof(message), "User %s has a balance of %s\n", user, balance);
                        write(sock, message, strlen(message));
                    } else {
                        write(sock, "Balance not found for the user.\n", strlen("Balance not found for the user.\n"));
                    }

                    found = 1;
                    break;  // Exit the loop once the user is found
                }

                // Reset the line buffer for the next line
                line_length = 0;
            }
        }

        if (found) {
            break;
        }
    }

    close(fd);

    if (!found) {
        write(sock, "User not found.\n", strlen("User not found.\n"));  // User not found
    }
}

// Helper function to log transactions and create file if it doesn't exist
void log_transaction(const char *username, const char *transaction_type, int amount) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_transactions.txt", username);  // Create file name for the user's transactions

    // Open file in append mode, create it if it doesn't exist
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open or create transaction log file");
        return;
    }

    // Get current time
   
     // Remove the newline character from ctime

    // Write the transaction details to the file
    fprintf(file, "%s,%d\n",transaction_type, amount);

    // Close the file
    fclose(file);
}



void depositMoney(int sock, const char *username) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  // Clear buffer
    write(sock, "Enter the amount to be deposited: ", strlen("Enter the amount to be deposited: "));

    // Read data from the socket
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }

    // Convert string to integer
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Deposit amount must be positive.\n", strlen("Deposit amount must be positive.\n"));
        return;
    }

    // Modify the balance
    if (modify_balance(sock, username, amount) == 0) {
        char message[256];
        snprintf(message, sizeof(message), "Deposited %d to %s's account successfully.\n", amount, username);
        write(sock, message, strlen(message));

        // Log the deposit transaction
        log_transaction(username, "Deposit", amount);
    } else {
        write(sock, "Failed to deposit money to account.\n", strlen("Failed to deposit money to account.\n"));
    }
}


void withdrawMoney(int sock, const char *username) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  // Clear buffer
    write(sock, "Enter the amount to be withdrawn: ", strlen("Enter the amount to be withdrawn: "));

    // Read data from the socket
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }

    // Convert string to integer
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Withdrawal amount must be positive.\n", strlen("Withdrawal amount must be positive.\n"));
        return;
    }

    // Modify the balance
    if (modify_balance(sock, username, -amount) == 0) {
        char message[256];
        snprintf(message, sizeof(message), "Withdrew %d from %s's account successfully.\n", amount, username);
        write(sock, message, strlen(message));

        // Log the withdrawal transaction
        log_transaction(username, "Withdrawal", amount);
    } else {
        write(sock, "Failed to withdraw money from account.\n", strlen("Failed to withdraw money from account.\n"));
    }
}


void transferfunds(int sock, const char *sender) {
    char buffer[256];
    bzero(buffer, sizeof(buffer));  // Clear the buffer

    char receiver[10];  // Allocate memory for the receiver
    bzero(receiver, sizeof(receiver));  // Clear receiver buffer

    // Prompt for receiver's name
    write(sock, "Enter receiver's name: ", strlen("Enter receiver's name: "));
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the string
    }

    // Copy the receiver's name
    strncpy(receiver, buffer, sizeof(receiver) - 1);
    receiver[sizeof(receiver) - 1] = '\0';  // Ensure null termination

    // Check if the username is valid (non-empty)
    if (strlen(receiver) == 0) {
        write(sock, "Invalid username received.\n", 28);
        return;
    }

    // Ask for the amount to be transferred
    write(sock, "Enter amount to be transferred: ", strlen("Enter amount to be transferred: "));
    bzero(buffer, sizeof(buffer));  // Clear buffer before reuse
    
    // Read the amount from the socket
    bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }
    
    // Convert the amount to an integer
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Transfer amount must be positive.\n", strlen("Transfer amount must be positive.\n"));
        return;
    }

    // Open the user file
    FILE *file = fopen("users.txt", "r+");
    if (file == NULL) {
        write(sock, "Failed to open user file.\n", strlen("Failed to open user file.\n"));
        return;
    }

    // Buffers for reading and writing user data
    char line[256];
    char new_content[4096] = "";  // To store modified file content
    int sender_balance = -1, receiver_balance = -1;
    int sender_found = 0, receiver_found = 0;
    char sender_line[800], receiver_line[800];  // To store sender/receiver lines for later update

    // Read each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char username[256], password[256], other_data[256];
        int balance;

        // Parse the user record
        sscanf(line, "%[^,],%[^,],%d,%[^\n]", username, password, &balance, other_data);

        // Check if this is the sender
        if (strcmp(username, sender) == 0) {
            if (balance < amount) {
                write(sock, "Insufficient funds in sender's account.\n", 40);
                fclose(file);
                return;
            }
            sender_balance = balance - amount;  // Deduct the amount
            sender_found = 1;
            snprintf(sender_line, sizeof(sender_line), "%s,%s,%d,%s\n", username, password, sender_balance, other_data);
        }

        // Check if this is the receiver
        else if (strcmp(username, receiver) == 0) {
            receiver_balance = balance + amount;  // Add the amount
            receiver_found = 1;
            snprintf(receiver_line, sizeof(receiver_line), "%s,%s,%d,%s\n", username, password, receiver_balance, other_data);
        } else {
            // Copy the original line if it's neither the sender nor the receiver
            strncat(new_content, line, sizeof(new_content) - strlen(new_content) - 1);
        }
    }

    // Check if both sender and receiver were found
    if (!sender_found) {
        write(sock, "Sender not found.\n", 18);
        fclose(file);
        return;
    }
    if (!receiver_found) {
        write(sock, "Receiver not found.\n", 20);
        fclose(file);
        return;
    }

    // Update the file with new balances for both sender and receiver
    fseek(file, 0, SEEK_SET);  // Move to the beginning of the file
    fprintf(file, "%s", new_content);  // Write the unchanged content
    fprintf(file, "%s", sender_line);  // Update sender's line
    fprintf(file, "%s", receiver_line);  // Update receiver's line
    fclose(file);  // Close the file

    // Log transactions for both users
    log_transaction(sender, "Transfer (sent)", amount);
    log_transaction(receiver, "Transfer (received)", amount);

    // Send a success message to the client
    char message[256];
    snprintf(message, sizeof(message), "Successfully transferred %d from %s to %s.\n", amount, sender, receiver);
    write(sock, message, strlen(message));
}



void apply_for_loan(int sock, const char *username) {
    char buffer[100];
    int loan_amount;

    write(sock, "Enter loan amount: ", 20);
    read(sock, buffer, sizeof(buffer));
    loan_amount = atoi(buffer);

    // Determine the next loan ID
    int next_loan_id = 1;
    int loan_fd = open("loans.txt", O_RDONLY);
    if (loan_fd >= 0) {
        char line[256];
        while (read(loan_fd, line, sizeof(line)) > 0) {
            next_loan_id++;
        }
        close(loan_fd);
    }

    // Append loan request to "loans.txt"
    loan_fd = open("loans.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (loan_fd < 0) {
        write(sock, "Error: Could not open loans file.\n", 35);
        return;
    }

    // Lock the loans file
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    if (fcntl(loan_fd, F_SETLK, &lock) == -1) {
        write(sock, "Error: Could not lock loans file.\n", 34);
        close(loan_fd);
        return;
    }

    char loan_entry[100];
    sprintf(loan_entry, "%d,%s,%d,Pending\n", next_loan_id, username, loan_amount);
    write(loan_fd, loan_entry, strlen(loan_entry));

    // Unlock the loans file
    lock.l_type = F_UNLCK;
    fcntl(loan_fd, F_SETLK, &lock);
    close(loan_fd);

    write(sock, "Loan application submitted successfully.\n", 41);
}


void add_feedback(int sock) {
    char feedback[256];

    write(sock, "Enter your feedback: ", 21);
    read(sock, feedback, sizeof(feedback));
    feedback[strcspn(feedback, "\n")] = '\0'; // Remove newline
    strtrim(feedback);
    int fd = open("feedback.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(sock, "Error: Could not open feedback file.\n", 37);
        return;
    }

    // Lock the feedback file
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(sock, "Error: Could not lock feedback file.\n", 37);
        close(fd);
        return;
    }

    char feedback_entry[300];
    sprintf(feedback_entry, "%s\n", feedback);
    write(fd, feedback_entry, strlen(feedback_entry));

    // Unlock the feedback file
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

}


void view_transaction_history(int sock, const char *username) {
    char buffer[1024], filename[MAX_LENGTH];
    sprintf(filename, "%s_transactions.txt", username);  // Transaction history file

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        write(sock, "No transaction history found.\n", 31);
        return;
    }

    // Lock the transaction history file
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  // Shared lock for reading
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(sock, "Error: Could not lock transaction history file.\n", 48);
        close(fd);
        return;
    }

    while (read(fd, buffer, sizeof(buffer)) > 0) {
        write(sock, buffer, strlen(buffer));  // Send transaction history to the client
    }

    // Unlock the transaction history file
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}


// Employee menu implementation
void employee_menu(int client_sock, const char *username) {
    char buffer[1024];
    int choice;

    while (1) {
        // Clear the buffer before sending the menu
        memset(buffer, 0, sizeof(buffer));

        // Send menu to client
        sprintf(buffer, "\nEmployee Menu:\n1. Add New Customer\n2. Modify Customer Details\n3. Process Loan Applications\n4. Approve/Reject Loans\n5. View Assigned Loans\n6. Logout\nEnter your choice: ");
        write(client_sock, buffer, strlen(buffer));
        printf("Menu sent to client.\n");

        // Clear the buffer before reading from client
        memset(buffer, 0, sizeof(buffer));

        // Read client input
        int bytes_read = read(client_sock, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the buffer
            printf("Received choice from client: %s\n", buffer);
        } else if (bytes_read == 0) {
            printf("Client disconnected.\n");
            return;
        } else {
            perror("Error reading from client");
            return;
        }

        choice = atoi(buffer);  // Convert the input to integer
        printf("Client chose option: %d\n", choice);

        switch (choice) {
            case 1:
                add_new_customer(client_sock);
                break;
            case 2:
                modify_customer_details(client_sock);
                break;
            case 3:
                process_loan_applications(client_sock);
                break;
            case 4:
                approve_reject_loan(client_sock);
                break;
            case 5:
                view_assigned_loans(client_sock, username);
                break;
            case 6:
                write(client_sock, "Logging out...\n", 15);
                logout(username);
                return;
            default:
                write(client_sock, "Invalid choice.\n", 16);
                printf("Invalid choice received.\n");
        }
    }
}


// --- Employee Functions --- //

// Login System: One session per user
int is_user_logged_in(const char *username, const char *role) {
    char *filename;

    // Determine which file to open based on the role
    
        filename = "sessions.txt";
    

    // Open the appropriate file for reading
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return 0;  // If the file doesn't exist or cannot be opened, assume the user is not logged in
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  // Shared lock for reading
    lock.l_whence = SEEK_SET;

    // Try to acquire a read lock on the file
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        close(fd);
        return 0;  // Could not acquire lock, assume the user is not logged in
    }

    // Buffer to read the file content
    char buffer[MAX_LENGTH];
    while (read(fd, buffer, sizeof(buffer)) > 0) {
        // Check if the username exists in the file
        if (strstr(buffer, username)) {
            // Release the lock and close the file
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return 1;  // User is logged in
        }
    }

    // Release the lock and close the file
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return 0;  // User is not logged in
}


void create_session(const char *username) {
    int fd = open("sessions.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) return;

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        close(fd);
        return;
    }

    write(fd, username, strlen(username));
    write(fd, "\n", 1);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

// Add New Customer
void add_new_customer(int client_sock) {
    char username[MAX_LENGTH], password[MAX_LENGTH], buffer[1024],hashed_password[MAX_LENGTH];
    int balance = 0;
    int bytes_read;
    write(client_sock, "Enter new customer username: ", 30);
   
bytes_read = read(client_sock, username, MAX_LENGTH);
    if (bytes_read > 0) {
        username[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    write(client_sock, "Enter new customer password: ", 30);
   bytes_read= read(client_sock, password, MAX_LENGTH);
 if (bytes_read > 0) {
        password[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  

    // Open customers.txt and append the new customer
    int fd = open("users.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open customers file.\n", 35);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }
    hash_password(password, hashed_password);
    sprintf(buffer, "%s,%s,%d,Active\n", username, hashed_password, balance);
    write(fd, buffer, strlen(buffer));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

}

// Modify Customer Details
void modify_customer_details(int client_sock) {
    char target_username[1024], new_username[1024], new_password[1024], buffer[1024], hashed_password[65];
    char line[10000], modified[10000] = {0};

    write(client_sock, "Enter customer username to modify: ", 35);
    read(client_sock, target_username, MAX_LENGTH);
    strtrim(target_username);  // Remove any extra whitespace
    write(client_sock, "Enter new username (or press Enter to skip): ", 44);
    read(client_sock, new_username, MAX_LENGTH);
    strtrim(new_username);
    write(client_sock, "Enter new password (or press Enter to skip): ", 44);
    read(client_sock, new_password, MAX_LENGTH);
    strtrim(new_password);

    int fd = open("users.txt", O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open customers file.\n", 35);
        return;
    }

    if (strlen(new_password) > 1) {
        hash_password(new_password, hashed_password);  // Hash password only if provided
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    FILE *fp = fdopen(fd, "r");  // Use `fdopen` only once to get a file pointer
    if (fp == NULL) {
        write(client_sock, "Error: Could not open file for reading.\n", 40);
        close(fd);
        return;
    }

    FILE *temp_file = fopen("temp_customers.txt", "w");
    if (temp_file == NULL) {
        write(client_sock, "Error: Cannot create temporary file.\n", 38);
        fclose(fp);
        close(fd);
        return;
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), fp) != NULL) {
        char username[1024], password[1024], balance[1024], status[1024];

        // Parse the line assuming comma-separated values
        sscanf(line, "%[^,],%[^,],%[^,],%s", username, password, balance, status);

        // Check if the current line matches the username to modify
        if (strcmp(username, target_username) == 0) {
            // Update username and password if provided
            if (strlen(new_username) > 1) {
                strncpy(username, new_username, sizeof(username));
                username[sizeof(username) - 1] = '\0';  // Ensure null-termination
            }
            if (strlen(new_password) > 1) {
                strncpy(password, hashed_password, sizeof(password));
                password[sizeof(password) - 1] = '\0';  // Ensure null-termination
            }

            // Construct the modified line
            snprintf(modified, sizeof(modified), "%s,%s,%s,%s\n", username, password, balance, status);

            // Write the modified line to the temporary file
            fwrite(modified, strlen(modified), 1, temp_file);
        } else {
            // Write the original line to the temporary file
            fwrite(line, strlen(line), 1, temp_file);
        }
    }

    // Unlock the file after reading and writing
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    fclose(fp);
    close(fd);
    fclose(temp_file);

    // Replace the original file with the updated file
    rename("temp_customers.txt", "users.txt");
}



// Process Loan Applications
void process_loan_applications(int client_sock) {
    char buffer[1024];
    int loan_id, amount;

    write(client_sock, "Enter loan ID to process: ", 26);
    read(client_sock, buffer, sizeof(buffer));
    loan_id = atoi(buffer);

    int fd = open("loans.txt", O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open loans file.\n", 32);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    char line[1024];
    while (read(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%d,%[^,],%d", &loan_id, buffer, &amount);
        if (loan_id == atoi(buffer)) {
            write(client_sock, "Loan found. Processing loan.\n", 30);
            // Perform loan processing here.
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    write(client_sock, "Loan not found.\n", 17);
}

// Approve/Reject Loans
void remove_loan_from_employee_file(int loan_id, const char *employee_username) {
    char employee_file[MAX_LENGTH], temp_file[MAX_LENGTH];
    snprintf(employee_file, sizeof(employee_file), "%s_loans.txt", employee_username);
    snprintf(temp_file, sizeof(temp_file), "temp_%s_loans.txt", employee_username);

    int fd_in = open(employee_file, O_RDONLY);
    if (fd_in < 0) {
        return;  // Employee loan file may not exist, skip if so
    }

    int fd_out = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        close(fd_in);
        return;
    }

    char line[1024];
    while (read(fd_in, line, sizeof(line)) > 0) {
        int id;
        sscanf(line, "%d", &id);
        // Copy all loans except the one we want to remove
        if (id != loan_id) {
            write(fd_out, line, strlen(line));
        }
    }

    close(fd_in);
    close(fd_out);

    // Replace the original file with the updated one
    rename(temp_file, employee_file);
}

void approve_reject_loan(int client_sock) {
    char buffer[MAX_LENGTH], status[MAX_LENGTH];
    int loan_id;

    // Get loan ID from the user
    write(client_sock, "Enter loan ID to approve/reject: ", 33);
    read(client_sock, buffer, MAX_LENGTH);
    loan_id = atoi(buffer);  // Convert input to an integer loan ID

    // Get approval or rejection status from the user
    write(client_sock, "Enter 'approve' or 'reject': ", 29);
    read(client_sock, status, MAX_LENGTH);
    status[strcspn(status, "\n")] = '\0';  // Remove newline from input

    if (strcmp(status, "approve") != 0 && strcmp(status, "reject") != 0) {
        write(client_sock, "Error: Invalid status. Use 'approve' or 'reject'.\n", 50);
        return;
    }

    // Open the loans file for reading and writing
    int fd = open("loans.txt", O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open loans file.\n", 32);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    char line[MAX_LENGTH];
    int found = 0;
    off_t offset = 0;
    char temp_filename[] = "temp_loans.txt";

    // Open the temporary file for writing
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    // Read each line from the loans file
    while (read(fd, line, sizeof(line) - 1) > 0) {
        line[strcspn(line, "\n")] = '\0';  // Null-terminate the line
        
        // Tokenize the line using commas
        char *token = strtok(line, ",");
        if (token == NULL) continue;

        int current_loan_id = atoi(token);  // First token is the loan ID
        token = strtok(NULL, ",");
        char username[MAX_LENGTH] = {0};
        if (token != NULL) {
            strncpy(username, token, sizeof(username) - 1);
            username[sizeof(username) - 1] = '\0';  // Null-terminate
        }
        token = strtok(NULL, ",");
        char loan_status[MAX_LENGTH] = {0};
        if (token != NULL) {
            strncpy(loan_status, token, sizeof(loan_status) - 1);
            loan_status[sizeof(loan_status) - 1] = '\0';  // Null-terminate
        }

        // Check if this is the loan we want to approve/reject
        if (current_loan_id == loan_id) {
            found = 1;
            // Update loan status
            strncpy(loan_status, status, sizeof(loan_status) - 1);
            loan_status[sizeof(loan_status) - 1] = '\0';  // Ensure null-termination
        }

        // Write the loan (updated or original) to the temporary file
        dprintf(temp_fd, "%d,%s,%s\n", current_loan_id, username, loan_status);
       if(found)
        remove_loan_from_employee_file(loan_id, username);
    
    }

   

    // Close files
    close(fd);
    close(temp_fd);

    // Rename the temporary file to the original loans file
    rename(temp_filename, "loans.txt");  // Replace the original file with the updated one

    // Unlock the file
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
}

// View Assigned Loan Applications
void view_assigned_loans(int client_sock, const char *username) {
    char emp_filename[256];
    snprintf(emp_filename, sizeof(emp_filename), "%s_loans.txt", username);

    int fd = open(emp_filename, O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employee's loan file.\n", 41);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  // Read lock to prevent simultaneous writes
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    char buffer[1024];
    int bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(client_sock, buffer, bytes_read);  // Send loan details to the client
    }

    lock.l_type = F_UNLCK;  // Unlock the file after reading
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

// Manager menu implementation
void manager_menu(int client_sock, const char *username) {
    char buffer[1024];
    int choice;

    while (1) {
        sprintf(buffer, "\nManager Menu:\n1. Activate/Deactivate Customer Accounts\n2. Assign Loan Application\n3. Review Customer Feedback\n4. Change Password\n5. Logout\nEnter your choice: ");
        write(client_sock, buffer, strlen(buffer));
        read(client_sock, buffer, sizeof(buffer));
        choice = atoi(buffer);

        switch (choice) {
            case 1:
                activate_deactivate_customer(client_sock);
                break;
            case 2:
                assign_loan_application(client_sock);
                break;
            case 3:
                review_customer_feedback(client_sock);
                break;
            case 4:
                change_password(client_sock, username,"Manager");
                break;
            case 5:
                write(client_sock, "Logging out...\n", 15);
                logout(username);
                return;
            default:
                write(client_sock, "Invalid choice.\n", 16);
        }
    }
}


// --- Manager Functions --- //

// Activate/Deactivate Customer Accounts
void activate_deactivate_customer(int client_sock) {
    char new_status[MAX_LENGTH] = {0}, username[MAX_LENGTH] = {0};
    char line[1024] = {0}, temp_filename[] = "temp_users.txt";

    // Get the username of the customer
    write(client_sock, "Enter the username of the customer: ", 36);
    read(client_sock, username, MAX_LENGTH);
    username[strcspn(username, "\n")] = '\0';  // Remove newline character

    // Get the new status (active/deactive) from the user
    write(client_sock, "Enter 'active' or 'deactive': ", strlen("Enter 'active' or 'deactive': "));
    read(client_sock, new_status, MAX_LENGTH);
    
    // Properly trim any leading/trailing whitespace or newline characters
    new_status[strcspn(new_status, "\n")] = '\0';  // Remove newline character
    strtrim(new_status);  // Trim leading and trailing spaces

    // Validate the status
    if (strcmp(new_status, "active") != 0 && strcmp(new_status, "deactive") != 0) {
        write(client_sock, "Error: Invalid status. Use 'active' or 'deactive'.\n", 52);
        return;
    }

    // Open the users file for reading
    int fd = open("users.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open users file.\n", 32);
        return;
    }

    // Read the entire line (all users in one line)
    ssize_t bytes_read = read(fd, line, sizeof(line) - 1);
    if (bytes_read <= 0) {
        write(client_sock, "Error: Cannot read users file.\n", 32);
        close(fd);
        return;
    }
    line[bytes_read] = '\0';  // Null-terminate the line

    // Open the temporary file for writing
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    char *token;
    char updated_line[MAX_LENGTH];

    // Initialize updated_line
    updated_line[0] = '\0';

    // Parse the file content and look for the username
    token = strtok(line, ",");
    int found = 0;

    while (token != NULL) {
        char *file_username = token;  // Current username
        char *password = strtok(NULL, ",");
        char *balance = strtok(NULL, ",");
        char *status = strtok(NULL, ",");  // This should be the status

        if (file_username != NULL && strcmp(file_username, username) == 0) {
            // Username matches, update the status
            snprintf(updated_line + strlen(updated_line), sizeof(updated_line) - strlen(updated_line),
                     "%s,%s,%s,%s", file_username, password, balance, new_status);
            found = 1;
        } else {
            // Keep the original user data
            snprintf(updated_line + strlen(updated_line), sizeof(updated_line) - strlen(updated_line),
                     "%s,%s,%s,%s", file_username, password, balance, status);
        }
        
        // Continue to the next set of user details
        token = strtok(NULL, ",");
    }

   
        write(temp_fd, updated_line, strlen(updated_line));  // Write the updated line to the temp file
    

    // Close files
    close(fd);
    close(temp_fd);

    // Rename the temp file to the original users file
    rename(temp_filename, "users.txt");  // Replace the original file with the updated one
}

void assign_loan_application(int client_sock) {
    char loan_id_str[120], employee_username[120], line[1024];
    int loan_id;

    // Request loan ID to assign
    write(client_sock, "Enter loan ID to assign: ", 25);
    read(client_sock, loan_id_str, sizeof(loan_id_str));
    loan_id = atoi(loan_id_str);

    // Read loans.txt to check available loans
    int fd = open("loans.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open loans file.\n", 32);
        return;
    }

    // Check if the loan ID exists
    int loan_exists = 0;
    while (read(fd, line, sizeof(line)) > 0) {
        int id;
        sscanf(line, "%d,", &id);
        if (id == loan_id) {
            loan_exists = 1;  // Loan ID found
            break;
        }
    }
    close(fd);

    if (!loan_exists) {
        write(client_sock, "Error: Loan ID does not exist.\n", 32);
        return;
    }

    // Open employees.txt to read all employees and count their loan assignments
    fd = open("employees.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    char least_employee[120];  // Store the username of the employee with least loans
    int least_count = INT_MAX; // Track the minimum number of loans

    // Read employee usernames from employees.txt
    while (read(fd, line, sizeof(line)) > 0) {
        char username[120], role[110], hashed_password[110];

        // Parse the employee entry (assuming format: username,hashed_password,role)
        sscanf(line, "%[^,],%[^,],%s", username, hashed_password, role);

        // Create the employee's loan file name
        char emp_filename[140];
        snprintf(emp_filename, sizeof(emp_filename), "%s_loans.txt", username);

        // Open the employee's loan file
        int emp_fd = open(emp_filename, O_RDONLY);
        if (emp_fd < 0) {
            continue;  // Skip if the employee loan file cannot be opened
        }

        // Count the number of loans assigned to this employee
        int count = 0;
        while (read(emp_fd, line, sizeof(line)) > 0) {
            count++;
        }
        close(emp_fd);

        // Update the employee with the least assigned loans
        if (count < least_count) {
            least_count = count;
            strncpy(least_employee, username, sizeof(least_employee)); // Store the employee with least loans
        }
    }
    close(fd);

    if (least_count == INT_MAX) {
        write(client_sock, "Error: No employees available.\n", 32);
        return;
    }

    // Assign the loan to the employee with the least assigned applications
    snprintf(employee_username, sizeof(employee_username), "%s", least_employee);
    printf("%s_loans.txt", employee_username);

    // Update the specific employee's loan file
    char emp_filename[130];
    snprintf(emp_filename, sizeof(emp_filename), "%s_loans.txt", employee_username);
    int emp_fd_update = open(emp_filename, O_WRONLY | O_APPEND);
    if (emp_fd_update < 0) {
        write(client_sock, "Error: Cannot open employee loans file.\n", 40);
        return;
    }

    // Write the assigned loan to the employee's file
    char assignment_line[MAX_LENGTH];
    snprintf(assignment_line, sizeof(assignment_line), "%d,Assigned Loan\n", loan_id);
    write(emp_fd_update, assignment_line, strlen(assignment_line));
    close(emp_fd_update);

    // Confirm assignment to the manager
}


// Review Customer Feedback
void review_customer_feedback(int client_sock) {
    char buffer[1024];
    int fd = open("feedback.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "No feedback available.\n", 23);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  // Shared lock for reading
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock feedback file.\n", 37);
        close(fd);
        return;
    }

    while (read(fd, buffer, sizeof(buffer)) > 0) {
        write(client_sock, buffer, strlen(buffer));  // Send feedback to the manager
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

// Change Password
void change_password(int client_sock, const char *username,  char *role) {
    char new_password[MAX_LENGTH], line[1024];
    char file_name[50];
    
    // Prompt for new password
    write(client_sock, "Enter new password: ", 20);
    int bytes_read = read(client_sock, new_password, sizeof(new_password));
    if (bytes_read > 0) {
        new_password[bytes_read] = '\0';  // Null-terminate
    }
    strtrim(new_password);  // Trim any extra spaces

    // Determine which file to open based on the role
    if (strcmp(role, "Administrator") == 0) {
        strcpy(file_name, "admin.txt");
    } else if (strcmp(role, "Customer") == 0) {
        strcpy(file_name, "users.txt");
    } else if (strcmp(role, "Manager") == 0) {
        strcpy(file_name, "manager.txt");
    } else if (strcmp(role, "Employee") == 0) {
        strcpy(file_name, "employees.txt");
    } else {
        write(client_sock, "Error: Invalid role.\n", 21);
        return;
    }

    // Open the appropriate file based on the role
    int fd = open(file_name, O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open file.\n", 26);
        return;
    }

    // Hash the new password
    char hashed_password[MAX_LENGTH];
    hash_password(new_password, hashed_password);

    // Lock the file for modification
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    // Create a temporary file for the updated data
    int temp_fd = open("temp_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    // Read line-by-line and update password if username matches
    ssize_t bytes_read_file;
    while ((bytes_read_file = read(fd, line, sizeof(line) - 1)) > 0) {
        line[bytes_read_file] = '\0';  // Null-terminate

        // If the line contains the username, modify the password
        if (strstr(line, username)) {
            char current_username[MAX_LENGTH];
            char current_password[MAX_LENGTH];
            char current_balance[MAX_LENGTH];
            char current_status[MAX_LENGTH];

            // Extract existing details
            sscanf(line, "%[^,],%[^,],%[^,],%s", current_username, current_password, current_balance, current_status);

            // Update the line with the new password
            snprintf(line, sizeof(line), "%s,%s,%s,%s\n", current_username, hashed_password, current_balance, current_status);
        }

        // Write the modified or original line to the temporary file
        write(temp_fd, line, strlen(line));
    }

    // Unlock the file and close the descriptors
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    // Replace the original file with the updated file
    rename("temp_file.txt", file_name);

    // Notify the client
    write(client_sock, "Password changed successfully.\n", 32);
}


// --- Administrator Functions --- //

// Login System: Check if user is already logged in


// Add New Bank Employee
void add_new_employee(int client_sock) {
    char username[MAX_LENGTH], password[MAX_LENGTH], role[MAX_LENGTH];
    char buffer[1024];
    char hashed_password[65];  // Buffer for the hashed password
    int bytes_read;

    // Clear buffers
    bzero(username, sizeof(username));
    bzero(password, sizeof(password));
    bzero(role, sizeof(role));
    bzero(buffer, sizeof(buffer));

    // Prompt for username and read it
    write(client_sock, "Enter new employee username: ", 30);
    bytes_read = read(client_sock, username, MAX_LENGTH - 1);  // Read username
    if (bytes_read > 0) {
        username[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    // Prompt for password and read it
    write(client_sock, "Enter new employee password: ", 30);
    bytes_read = read(client_sock, password, MAX_LENGTH - 1);  // Read password
    if (bytes_read > 0) {
        password[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  

    // Prompt for role and read it
    write(client_sock, "Enter role (e.g., Employee, Manager): ", 39);
    bytes_read = read(client_sock, role, MAX_LENGTH - 1);  // Read role
    if (bytes_read > 0) {
        role[bytes_read] = '\0';  // Null-terminate the string
    }
    strtrim(role);
    role[strcspn(role, "\n")] = '\0';  

    // Open employees.txt for appending the new employee's details
    int fd = open("employees.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    // Apply file lock to prevent race conditions
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    // Hash the password before storing it
    hash_password(password, hashed_password);

    // Write the new employee's details to the file, all in one line
    snprintf(buffer, sizeof(buffer), "%s,%s,%s\n", username, hashed_password, role);
    write(fd, buffer, strlen(buffer));  // Write the line to the file

    // Unlock the file and close it
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    // Create a username_loans.txt file for the new employee
    char loans_filename[110];
    snprintf(loans_filename, sizeof(loans_filename), "%s_loans.txt", username);
    int loans_fd = open(loans_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (loans_fd < 0) {
        write(client_sock, "Error: Cannot create loans file for the employee.\n", 51);
        return;
    }
    close(loans_fd);  // Close the loans file after creation

    // Notify the client that the employee was added successfully
}

// Modify Customer/Employee Details
void modify_user_details(int client_sock) {
    char target_username[MAX_LENGTH], new_username[MAX_LENGTH], new_password[MAX_LENGTH], new_role[MAX_LENGTH];
    char line[1024], temp_line[1024];
    char current_username[MAX_LENGTH], current_password[MAX_LENGTH], current_role[MAX_LENGTH], hashed_password[64];

    // Read input from the client
    write(client_sock, "Enter username to modify: ", 25);
    int bytes_read = read(client_sock, target_username, MAX_LENGTH);
    target_username[bytes_read] = '\0';  // Null-terminate
    strtrim(target_username);
    target_username[strcspn(target_username, "\n")] = '\0';  // Remove newline if any

    write(client_sock, "Enter new username (or press Enter to skip): ", 44);
    bytes_read = read(client_sock, new_username, MAX_LENGTH);
    new_username[bytes_read] = '\0';  // Null-terminate
    strtrim(new_username);
    new_username[strcspn(new_username, "\n")] = '\0';  // Remove newline if any

    write(client_sock, "Enter new password (or press Enter to skip): ", 44);
    bytes_read = read(client_sock, new_password, MAX_LENGTH);
    new_password[bytes_read] = '\0';  // Null-terminate
    strtrim(new_password);
    new_password[strcspn(new_password, "\n")] = '\0';  // Remove newline if any

    write(client_sock, "Enter new role (or press Enter to skip): ", 42);
    bytes_read = read(client_sock, new_role, MAX_LENGTH);
    new_role[bytes_read] = '\0';  // Null-terminate
    strtrim(new_role);
    new_role[strcspn(new_role, "\n")] = '\0';  // Remove newline if any

    // Open the employees.txt file
    int fd = open("employees.txt", O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    // Lock the file for modification
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    // Create a temporary file for the updated data
    int temp_fd = open("temp_employees.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    // Read line-by-line from employees.txt
    ssize_t bytes_read_file;
    char *line_ptr;
    while ((bytes_read_file = read(fd, line, sizeof(line) - 1)) > 0) {
        line[bytes_read_file] = '\0';  // Null-terminate the buffer
        line_ptr = line;

        char *newline_ptr;
        // Process each line individually
        while ((newline_ptr = strchr(line_ptr, '\n')) != NULL) {
            *newline_ptr = '\0';  // Null-terminate at the newline

            // Extract current username, password, and role
            sscanf(line_ptr, "%[^,],%[^,],%s\n", current_username, current_password, current_role);
            strtrim(current_username);  // Trim any extra spaces in the username

            // If this is the user to be modified
            if (strcmp(current_username, target_username) == 0) {
                // Update details if new values are provided
                if (strlen(new_username) > 0) strcpy(current_username, new_username);
                if (strlen(new_password) > 0) {
                    hash_password(new_password, hashed_password);  // Hash the new password
                    strcpy(current_password, hashed_password);
                }
                if (strlen(new_role) > 0) strcpy(current_role, new_role);

                // If the new role is Manager, update or create manager.txt
                if (strcmp(current_role, "Manager") == 0) {
                    int manager_fd = open("manager.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (manager_fd >= 0) {
                        char manager_entry[512];
                        snprintf(manager_entry, sizeof(manager_entry), "%s,%s,%s\n", current_username, current_password, current_role);
                        write(manager_fd, manager_entry, strlen(manager_entry));
                        close(manager_fd);
                    } else {
                        write(client_sock, "Error: Could not open or create manager file.\n", 47);
                    }
                }

                // Update the current line with the new details
                snprintf(temp_line, sizeof(temp_line), "%s,%s,%s\n", current_username, current_password, current_role);
            } else {
                // If not the user being modified, keep the original line
                strncpy(temp_line, line_ptr, sizeof(temp_line));
                strncat(temp_line, "\n", sizeof(temp_line) - strlen(temp_line) - 1);  // Ensure newline is added back
            }

            // Write the updated or original line to the temporary file
            write(temp_fd, temp_line, strlen(temp_line));

            // Move pointer to the next line
            line_ptr = newline_ptr + 1;
        }
    }

    // Unlock the employees.txt file and close the file descriptors
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    // Replace the original employees.txt file with the updated version
    rename("temp_employees.txt", "employees.txt");

    // Notify the client of success
    
}




// Manage User Roles
void manage_user_roles(int client_sock) {
    char buffer[1024];
    int fd = open("employees.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  // Shared lock for reading
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    // Read roles and send to client
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer
        write(client_sock, buffer, bytes_read);
    }

    // Send a termination signal
    const char* end_signal = "stop\0";
    write(client_sock, end_signal, strlen(end_signal));

    lock.l_type = F_UNLCK;  // Unlock the file
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
}



// Change Password


void admin_menu(int client_sock, const char *username) {
    char buffer[1024];
    int choice;

    while (1) {
        sprintf(buffer, "\nAdministrator Menu:\n1. Add New Bank Employee\n2. Modify Customer/Employee Details\n3. Manage User Roles\n4. Change Password\n5. Logout\nEnter your choice: ");
        write(client_sock, buffer, strlen(buffer));
        printf("MENU SENT\n");
        read(client_sock, buffer, sizeof(buffer));
        choice = atoi(buffer);

        switch (choice) {
            case 1:
                add_new_employee(client_sock);
                break;
            case 2:
                modify_user_details(client_sock);
                break;
            case 3:
                manage_user_roles(client_sock);
                break;
            case 4:
                change_password(client_sock, username,"Administrator");
                break;
            case 5:
                write(client_sock, "Logging out...\n", 15);
                logout(username);
                return;
            default:
                write(client_sock, "Invalid choice.\n", 16);
        }
    }
}

