#include"roleselection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<ctype.h>



static void strtrim(char *str) {
    char *end;

    
    while (isspace((unsigned char)*str)) str++;

    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    
    *(end + 1) = '\0';
}
void logout(const char *username) {
    int fd = open("sessions.txt", O_RDWR);
    if (fd < 0) {
        return;  
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        close(fd);
        return;  
    }

    char buffer[MAX_LENGTH];
    int temp_fd = open("temp_sessions.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    while (read(fd, buffer, sizeof(buffer)) > 0) {
        if (!strstr(buffer, username)) {
            write(temp_fd, buffer, strlen(buffer));  
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    rename("temp_sessions.txt", "sessions.txt");  
}


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

    
    
       
        
        
         
          
          
          
          
        
   

    
    return -1;  
}


void customer_menu(int sock, const char *username) {
    char buffer[1024];

    while (1) {
        
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
        printf("Menu sent to client.\n"); 

        memset(buffer, 0, sizeof(buffer));  

        ssize_t bytesRead = read(sock, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            printf("Connection closed by client or error occurred.\n");
            break;
        }

        buffer[bytesRead] = '\0';  

        
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
                write(sock,"Logging out...\n",strlen("Logging out..."));
                return;           
            
            default:
                write(sock, "Invalid choice. Please try again.\n", 34);
                break;
        }
    }

    close(sock);
}





int lock_file(int fd, int lock_type, off_t offset, size_t size) {
    printf("INSIDE LOCK FILE %d",fd);
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = lock_type;  
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;  
    lock.l_len = size;      

    return fcntl(fd, F_SETLKW, &lock);  
}


int unlock_file(int fd, off_t offset, size_t size) {
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_UNLCK;  
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;  
    lock.l_len = size;      

    return fcntl(fd, F_SETLKW, &lock);  
}








int modify_balance(int sock, const char *username, int amount) {
    int fd = open("users.txt", O_RDONLY);  
    if (fd < 0) {
        return -1;  
    }

    struct User user;
    char buffer[1024];  
    ssize_t bytes_read;
    size_t total_bytes = 0;

    
    while ((bytes_read = read(fd, buffer + total_bytes, sizeof(buffer) - total_bytes - 1)) > 0) {
        total_bytes += bytes_read;

        
        if (total_bytes >= sizeof(buffer) - 1) {
            break;
        }
    }
    close(fd);  

    buffer[total_bytes] = '\0';  

    
    int tmp_fd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tmp_fd < 0) {
        return -1;  
    }

    
    char *record = strtok(buffer, "\n");
    int balance_updated = 0;

    
    while (record != NULL) {
        
        memset(&user, 0, sizeof(user));  

        
        int parsed = sscanf(record, "%[^,],%[^,],%d,%[^,\n]", user.username, user.password, &user.balance, user.status);
        if (parsed == 4) {  

            
            if (strcmp(username, user.username) == 0) {
                
                if (strcmp(user.status, "inactive") == 0) {
                    close(tmp_fd);
                    return -1;  
                }

                
                if (amount < 0 && user.balance < (-amount)) {
                    close(tmp_fd);
                    return -1;  
                }

                
                user.balance += amount;
                balance_updated = 1;  
            }

            
            dprintf(tmp_fd, "%s,%s,%d,%s\n", user.username, user.password, user.balance, user.status);
        }

        
        record = strtok(NULL, "\n");  
    }

    close(tmp_fd);  

    
    rename("temp.txt", "users.txt");

    return balance_updated ? 0 : -1;  
}




void get_balance(int sock, const char *username) {
    int fd = open("users.txt", O_RDONLY);  
    if (fd < 0) {
        write(sock, "Failed to open file.\n", strlen("Failed to open file.\n"));
        return;
    }

    char buffer[400];
    char line_buffer[1024];  
    ssize_t bytes_read;
    size_t line_length = 0;  
    int found = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  

        
        for (size_t i = 0; i < bytes_read; i++) {
            
            if (line_length < sizeof(line_buffer) - 1) {
                line_buffer[line_length++] = buffer[i];
            }

            
            if (buffer[i] == '\n') {
                line_buffer[line_length] = '\0';  

                
                char *token = strtok(line_buffer, ",");
                char *user = token;  
                token = strtok(NULL, ",");  
                token = strtok(NULL, ",");  
                char *balance = token;  
                token = strtok(NULL, ",");  

                
                if (user && strcmp(username, user) == 0) {
                    
                    if (balance) {
                        char message[100];
                        snprintf(message, sizeof(message), "User %s has a balance of %s\n", user, balance);
                        write(sock, message, strlen(message));
                    } else {
                        write(sock, "Balance not found for the user.\n", strlen("Balance not found for the user.\n"));
                    }

                    found = 1;
                    break;  
                }

                
                line_length = 0;
            }
        }

        if (found) {
            break;
        }
    }

    close(fd);

    if (!found) {
        write(sock, "User not found.\n", strlen("User not found.\n"));  
    }
}


void log_transaction(const char *username, const char *transaction_type, int amount) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_transactions.txt", username);  

    
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open or create transaction log file");
        return;
    }

    
   
     

    
    fprintf(file, "%s,%d\n",transaction_type, amount);

    
    fclose(file);
}



void depositMoney(int sock, const char *username) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  
    write(sock, "Enter the amount to be deposited: ", strlen("Enter the amount to be deposited: "));

    
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }

    
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Deposit amount must be positive.\n", strlen("Deposit amount must be positive.\n"));
        return;
    }

    
    if (modify_balance(sock, username, amount) == 0) {
        char message[256];
        snprintf(message, sizeof(message), "Deposited %d to %s's account successfully.\n", amount, username);
        write(sock, message, strlen(message));

        
        log_transaction(username, "Deposit", amount);
    } else {
        write(sock, "Failed to deposit money to account.\n", strlen("Failed to deposit money to account.\n"));
    }
}


void withdrawMoney(int sock, const char *username) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  
    write(sock, "Enter the amount to be withdrawn: ", strlen("Enter the amount to be withdrawn: "));

    
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }

    
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Withdrawal amount must be positive.\n", strlen("Withdrawal amount must be positive.\n"));
        return;
    }

    
    if (modify_balance(sock, username, -amount) == 0) {
        char message[256];
        snprintf(message, sizeof(message), "Withdrew %d from %s's account successfully.\n", amount, username);
        write(sock, message, strlen(message));

        
        log_transaction(username, "Withdrawal", amount);
    } else {
        write(sock, "Failed to withdraw money from account.\n", strlen("Failed to withdraw money from account.\n"));
    }
}


void transferfunds(int sock, const char *sender) {
    char buffer[256];
    bzero(buffer, sizeof(buffer));  

    char receiver[10];  
    bzero(receiver, sizeof(receiver));  

    
    write(sock, "Enter receiver's name: ", strlen("Enter receiver's name: "));
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  
    }

    
    strncpy(receiver, buffer, sizeof(receiver) - 1);
    receiver[sizeof(receiver) - 1] = '\0';  

    
    if (strlen(receiver) == 0) {
        write(sock, "Invalid username received.\n", 28);
        return;
    }

    
    write(sock, "Enter amount to be transferred: ", strlen("Enter amount to be transferred: "));
    bzero(buffer, sizeof(buffer));  
    
    
    bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        write(sock, "Failed to read amount from socket.\n", 35);
        return;
    }
    
    
    int amount = atoi(buffer);
    if (amount <= 0) {
        write(sock, "Transfer amount must be positive.\n", strlen("Transfer amount must be positive.\n"));
        return;
    }

    
    FILE *file = fopen("users.txt", "r+");
    if (file == NULL) {
        write(sock, "Failed to open user file.\n", strlen("Failed to open user file.\n"));
        return;
    }

    
    char line[256];
    char new_content[4096] = "";  
    int sender_balance = -1, receiver_balance = -1;
    int sender_found = 0, receiver_found = 0;
    char sender_line[800], receiver_line[800];  

    
    while (fgets(line, sizeof(line), file) != NULL) {
        char username[256], password[256], other_data[256];
        int balance;

        
        sscanf(line, "%[^,],%[^,],%d,%[^\n]", username, password, &balance, other_data);

        
        if (strcmp(username, sender) == 0) {
            if (balance < amount) {
                write(sock, "Insufficient funds in sender's account.\n", 40);
                fclose(file);
                return;
            }
            sender_balance = balance - amount;  
            sender_found = 1;
            snprintf(sender_line, sizeof(sender_line), "%s,%s,%d,%s\n", username, password, sender_balance, other_data);
        }

        
        else if (strcmp(username, receiver) == 0) {
            receiver_balance = balance + amount;  
            receiver_found = 1;
            snprintf(receiver_line, sizeof(receiver_line), "%s,%s,%d,%s\n", username, password, receiver_balance, other_data);
        } else {
            
            strncat(new_content, line, sizeof(new_content) - strlen(new_content) - 1);
        }
    }

    
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

    
    fseek(file, 0, SEEK_SET);  
    fprintf(file, "%s", new_content);  
    fprintf(file, "%s", sender_line);  
    fprintf(file, "%s", receiver_line);  
    fclose(file);  

    
    log_transaction(sender, "Transfer (sent)", amount);
    log_transaction(receiver, "Transfer (received)", amount);

    
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

    
    int next_loan_id = 1;
    int loan_fd = open("loans.txt", O_RDONLY);
    if (loan_fd >= 0) {
        char line[256];
        while (read(loan_fd, line, sizeof(line)) > 0) {
            next_loan_id++;
        }
        close(loan_fd);
    }

    
    loan_fd = open("loans.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (loan_fd < 0) {
        write(sock, "Error: Could not open loans file.\n", 35);
        return;
    }

    
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

    
    lock.l_type = F_UNLCK;
    fcntl(loan_fd, F_SETLK, &lock);
    close(loan_fd);

    write(sock, "Loan application submitted successfully.\n", 41);
}


void add_feedback(int sock) {
    char feedback[256];

    write(sock, "Enter your feedback: ", 21);
    int bytes=read(sock, feedback, sizeof(feedback));
    feedback[bytes] = '\0'; 
    strtrim(feedback);
    int fd = open("feedback.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(sock, "Error: Could not open feedback file.\n", 37);
        return;
    }

    
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

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

}


void view_transaction_history(int sock, const char *username) {
    char buffer[1024], filename[MAX_LENGTH];
    sprintf(filename, "%s_transactions.txt", username);  

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        write(sock, "No transaction history found.\n", 31);
        return;
    }

    
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(sock, "Error: Could not lock transaction history file.\n", 48);
        close(fd);
        return;
    }

    while (read(fd, buffer, sizeof(buffer)) > 0) {
        write(sock, buffer, strlen(buffer));  
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}



void employee_menu(int client_sock, const char *username) {
    char buffer[1024];
    int choice;

    while (1) {
        
        memset(buffer, 0, sizeof(buffer));

        
        sprintf(buffer, "\nEmployee Menu:\n1. Add New Customer\n2. Modify Customer Details\n3. Process Loan Applications\n4. Approve/Reject Loans\n5. View Assigned Loans\n6. Logout\nEnter your choice: ");
        write(client_sock, buffer, strlen(buffer));
        printf("Menu sent to client.\n");

        
        memset(buffer, 0, sizeof(buffer));

        
        int bytes_read = read(client_sock, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  
            printf("Received choice from client: %s\n", buffer);
        } else if (bytes_read == 0) {
            printf("Client disconnected.\n");
            return;
        } else {
            perror("Error reading from client");
            return;
        }

        choice = atoi(buffer);  
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





int is_user_logged_in(const char *username, const char *role) {
    char *filename;

        filename = "sessions.txt";

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return 0;  
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  
    lock.l_whence = SEEK_SET;

    
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        close(fd);
        return 0;  
    }

    
    char buffer[MAX_LENGTH];
    while (read(fd, buffer, sizeof(buffer)) > 0) {
        
        if (strstr(buffer, username)) {
            
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return 1;  
        }
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return 0;  
}


void create_session(const char *username) {
    int fd = open("sessions.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) return;

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  
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


void add_new_customer(int client_sock) {
    char username[MAX_LENGTH], password[MAX_LENGTH], buffer[1024],hashed_password[MAX_LENGTH];
    int balance = 0;
    int bytes_read;
    write(client_sock, "Enter new customer username: ", 30);
   
bytes_read = read(client_sock, username, MAX_LENGTH);
    if (bytes_read > 0) {
        username[bytes_read] = '\0';  
    }
    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    write(client_sock, "Enter new customer password: ", 30);
   bytes_read= read(client_sock, password, MAX_LENGTH);
 if (bytes_read > 0) {
        password[bytes_read] = '\0';  
    }
    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  

    
    int fd = open("users.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open customers file.\n", 35);
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
    hash_password(password, hashed_password);
    sprintf(buffer, "%s,%s,%d,Active\n", username, hashed_password, balance);
    write(fd, buffer, strlen(buffer));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

}


void modify_customer_details(int client_sock) {
    char target_username[1024], new_username[1024], new_password[1024], buffer[1024], hashed_password[65];
    char line[10000], modified[10000] = {0};

    write(client_sock, "Enter customer username to modify: ", 35);
    read(client_sock, target_username, MAX_LENGTH);
    strtrim(target_username);  
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
        hash_password(new_password, hashed_password);  
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

    FILE *fp = fdopen(fd, "r");  
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

    
    while (fgets(line, sizeof(line), fp) != NULL) {
        char username[1024], password[1024], balance[1024], status[1024];

        
        sscanf(line, "%[^,],%[^,],%[^,],%s", username, password, balance, status);

        
        if (strcmp(username, target_username) == 0) {
            
            if (strlen(new_username) > 1) {
                strncpy(username, new_username, sizeof(username));
                username[sizeof(username) - 1] = '\0';  
            }
            if (strlen(new_password) > 1) {
                strncpy(password, hashed_password, sizeof(password));
                password[sizeof(password) - 1] = '\0';  
            }

            
            snprintf(modified, sizeof(modified), "%s,%s,%s,%s\n", username, password, balance, status);

            
            fwrite(modified, strlen(modified), 1, temp_file);
        } else {
            
            fwrite(line, strlen(line), 1, temp_file);
        }
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    fclose(fp);
    close(fd);
    fclose(temp_file);

    
    rename("temp_customers.txt", "users.txt");
}



void process_loan_applications(int client_sock) {
    char buffer[1024];
    char loan_id_str[100], status[100];

    
    write(client_sock, "Enter loan ID to process: ", 26);
    read(client_sock, buffer, sizeof(buffer));
    int search_loan_id = atoi(buffer);

    
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
    int found = 0;
    int bytes_read;
    while ( bytes_read=read(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%[^,],%s", loan_id_str, status);
        int loan_id = atoi(loan_id_str);
        if (loan_id == search_loan_id) {
                    buffer[bytes_read] = '\0';  

            write(client_sock, "Loan found. Processing loan.\n", 30);
            found = 1;
            break;
        }
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    if (!found) {
        write(client_sock, "Loan not found.\n", 17);
    }
}


void remove_loan_from_employee_file(int loan_id, const char *employee_username) {
    char employee_file[MAX_LENGTH], temp_file[MAX_LENGTH];
    snprintf(employee_file, sizeof(employee_file), "%s_loans.txt", employee_username);
    snprintf(temp_file, sizeof(temp_file), "temp_%s_loans.txt", employee_username);

    int fd_in = open(employee_file, O_RDONLY);
    if (fd_in < 0) {
        return;  
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
        
        if (id != loan_id) {
            write(fd_out, line, strlen(line));
        }
    }

    close(fd_in);
    close(fd_out);

    
    rename(temp_file, employee_file);
}

void approve_reject_loan(int client_sock) {
    char buffer[MAX_LENGTH], status[MAX_LENGTH];
    int loan_id;

    
    write(client_sock, "Enter loan ID to approve/reject: ", 33);
    read(client_sock, buffer, MAX_LENGTH);
    loan_id = atoi(buffer);  

    
    write(client_sock, "Enter 'approve' or 'reject': ", 29);
    read(client_sock, status, MAX_LENGTH);
    status[strcspn(status, "\n")] = '\0';  

    if (strcmp(status, "approve") != 0 && strcmp(status, "reject") != 0) {
        write(client_sock, "Error: Invalid status. Use 'approve' or 'reject'.\n", 50);
        return;
    }

    
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

    char line[MAX_LENGTH];
    int found = 0;
    off_t offset = 0;
    char temp_filename[] = "temp_loans.txt";

    
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    
    while (read(fd, line, sizeof(line) - 1) > 0) {
        line[strcspn(line, "\n")] = '\0';  
        
        
        char *token = strtok(line, ",");
        if (token == NULL) continue;

        int current_loan_id = atoi(token);  
        token = strtok(NULL, ",");
        char username[MAX_LENGTH] = {0};
        if (token != NULL) {
            strncpy(username, token, sizeof(username) - 1);
            username[sizeof(username) - 1] = '\0';  
        }
        token = strtok(NULL, ",");
        char loan_status[MAX_LENGTH] = {0};
        if (token != NULL) {
            strncpy(loan_status, token, sizeof(loan_status) - 1);
            loan_status[sizeof(loan_status) - 1] = '\0';  
        }

        
        if (current_loan_id == loan_id) {
            found = 1;
            
            strncpy(loan_status, status, sizeof(loan_status) - 1);
            loan_status[sizeof(loan_status) - 1] = '\0';  
        }

        
        dprintf(temp_fd, "%d,%s,%s\n", current_loan_id, username, loan_status);
       if(found)
        remove_loan_from_employee_file(loan_id, username);
    
    }

   

    
    close(fd);
    close(temp_fd);

    
    rename(temp_filename, "loans.txt");  

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
}


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
    lock.l_type = F_RDLCK;  
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    char buffer[1024];
    int bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        buffer[bytes_read] = '\0';  
        write(client_sock, buffer, bytes_read);  
    }
    write(client_sock, "Type Ok\n", strlen("Type Ok\n"));  
    read(client_sock,buffer,sizeof(buffer));
    lock.l_type = F_UNLCK;  
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}


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





void activate_deactivate_customer(int client_sock) {
    char new_status[MAX_LENGTH] = {0}, username[MAX_LENGTH] = {0};
    char line[1024] = {0}, temp_filename[] = "temp_users.txt";

    
    write(client_sock, "Enter the username of the customer: ", 36);
    read(client_sock, username, MAX_LENGTH);
    username[strcspn(username, "\n")] = '\0';  

    write(client_sock, "Enter 'active' or 'deactive': ", strlen("Enter 'active' or 'deactive': "));
    read(client_sock, new_status, MAX_LENGTH);
    new_status[strcspn(new_status, "\n")] = '\0';  
    strtrim(new_status);

    
    if (strcmp(new_status, "active") != 0 && strcmp(new_status, "deactive") != 0) {
        write(client_sock, "Error: Invalid status. Use 'active' or 'deactive'.\n", 52);
        return;
    }

    
    int fd = open("users.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening users file");
        write(client_sock, "Error: Cannot open users file.\n", 32);
        return;
    }

    
    ssize_t bytes_read = read(fd, line, sizeof(line) - 1);
    if (bytes_read <= 0) {
        perror("Error reading users file");
        write(client_sock, "Error: Cannot read users file.\n", 32);
        close(fd);
        return;
    }
    line[bytes_read] = '\0';  
    printf("Read from file:\n%s\n", line);  

    
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        perror("Error creating temp file");
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    int found = 0;
    char *line_token = strtok(line, "\n");  

    
    while (line_token != NULL) {
        
        char *file_username = strtok(line_token, ",");
        char *password = strtok(NULL, ",");
        char *balance = strtok(NULL, ",");
        char *status = strtok(NULL, "");  

        if (file_username != NULL) {
            char updated_line[MAX_LENGTH] = {0};  

            if (strcmp(file_username, username) == 0) {
                
                snprintf(updated_line, sizeof(updated_line), "%s,%s,%s,%s\n",
                         file_username, password, balance, new_status);
                found = 1;
            } else {
                
                snprintf(updated_line, sizeof(updated_line), "%s,%s,%s,%s\n",
                         file_username, password, balance, status);
            }

            
            write(temp_fd, updated_line, strlen(updated_line));
            printf("Writing to temp file: %s", updated_line);  
        }

        
        line_token = strtok(NULL, "\n");
    }

    if (!found) {
        write(client_sock, "Error: Username not found.\n", 28);
    }

    close(fd);
    close(temp_fd);

    
    rename(temp_filename, "users.txt");
}
    

    
 

void assign_loan_application(int client_sock) {
    char loan_id_str[120], employee_username[120], line[1024];
    int loan_id;

    
    write(client_sock, "Enter loan ID to assign: ", 25);
    read(client_sock, loan_id_str, sizeof(loan_id_str));
    loan_id = atoi(loan_id_str);

    
    int fd = open("loans.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open loans file.\n", 32);
        return;
    }

    
    int loan_exists = 0;
    while (read(fd, line, sizeof(line)) > 0) {
        int id;
        sscanf(line, "%d,", &id);
        if (id == loan_id) {
            loan_exists = 1;  
            break;
        }
    }
    close(fd);

    if (!loan_exists) {
        write(client_sock, "Error: Loan ID does not exist.\n", 32);
        return;
    }

    
    fd = open("employees.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    char least_employee[120];  
    int least_count = INT_MAX; 

    
    while (read(fd, line, sizeof(line)) > 0) {
        char username[120], role[110], hashed_password[110];

        
        sscanf(line, "%[^,],%[^,],%s", username, hashed_password, role);

        
        char emp_filename[140];
        snprintf(emp_filename, sizeof(emp_filename), "%s_loans.txt", username);

        
        int emp_fd = open(emp_filename, O_RDONLY);
        if (emp_fd < 0) {
            continue;  
        }

        
        int count = 0;
        while (read(emp_fd, line, sizeof(line)) > 0) {
            count++;
        }
        close(emp_fd);

        
        if (count < least_count) {
            least_count = count;
            strncpy(least_employee, username, sizeof(least_employee)); 
        }
    }
    close(fd);

    if (least_count == INT_MAX) {
        write(client_sock, "Error: No employees available.\n", 32);
        return;
    }

    
    snprintf(employee_username, sizeof(employee_username), "%s", least_employee);
    printf("%s_loans.txt", employee_username);

    
    char emp_filename[130];
    snprintf(emp_filename, sizeof(emp_filename), "%s_loans.txt", employee_username);
    int emp_fd_update = open(emp_filename, O_WRONLY | O_APPEND);
    if (emp_fd_update < 0) {
        write(client_sock, "Error: Cannot open employee loans file.\n", 40);
        return;
    }

    
    char assignment_line[MAX_LENGTH];
    snprintf(assignment_line, sizeof(assignment_line), "%d,Assigned Loan\n", loan_id);
    write(emp_fd_update, assignment_line, strlen(assignment_line));
    close(emp_fd_update);

    
}



void review_customer_feedback(int client_sock) {
    char buffer[1024];
    int fd = open("feedback.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "No feedback available.\n", 23);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock feedback file.\n", 37);
        close(fd);
        return;
    }

    while (read(fd, buffer, sizeof(buffer)) > 0) {
        write(client_sock, buffer, strlen(buffer));  
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}


void change_password(int client_sock, const char *username,  char *role) {
    char new_password[MAX_LENGTH], line[1024];
    char file_name[50];
    
    
    write(client_sock, "Enter new password: ", 20);
    int bytes_read = read(client_sock, new_password, sizeof(new_password));
    if (bytes_read > 0) {
        new_password[bytes_read] = '\0';  
    }
    strtrim(new_password);  

    
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

    
    int fd = open(file_name, O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open file.\n", 26);
        return;
    }

    
    char hashed_password[MAX_LENGTH];
    hash_password(new_password, hashed_password);

    
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    
    int temp_fd = open("temp_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    
    ssize_t bytes_read_file;
    while ((bytes_read_file = read(fd, line, sizeof(line) - 1)) > 0) {
        line[bytes_read_file] = '\0';  

        
        if (strstr(line, username)) {
            char current_username[MAX_LENGTH];
            char current_password[MAX_LENGTH];
            char current_balance[MAX_LENGTH];
            char current_status[MAX_LENGTH];

            
            sscanf(line, "%[^,],%[^,],%[^,],%s", current_username, current_password, current_balance, current_status);

            
            snprintf(line, sizeof(line), "%s,%s,%s,%s\n", current_username, hashed_password, current_balance, current_status);
        }

        
        write(temp_fd, line, strlen(line));
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    
    rename("temp_file.txt", file_name);

    
}








void add_new_employee(int client_sock) {
    char username[MAX_LENGTH], password[MAX_LENGTH], role[MAX_LENGTH];
    char buffer[1024];
    char hashed_password[65];  
    int bytes_read;

    
    bzero(username, sizeof(username));
    bzero(password, sizeof(password));
    bzero(role, sizeof(role));
    bzero(buffer, sizeof(buffer));

    
    write(client_sock, "Enter new employee username: ", 30);
    bytes_read = read(client_sock, username, MAX_LENGTH - 1);  
    if (bytes_read > 0) {
        username[bytes_read] = '\0';  
    }
    strtrim(username);
    username[strcspn(username, "\n")] = '\0';  

    
    write(client_sock, "Enter new employee password: ", 30);
    bytes_read = read(client_sock, password, MAX_LENGTH - 1);  
    if (bytes_read > 0) {
        password[bytes_read] = '\0';  
    }
    strtrim(password);
    password[strcspn(password, "\n")] = '\0';  

    
    write(client_sock, "Enter role (e.g., Employee, Manager): ", 39);
    bytes_read = read(client_sock, role, MAX_LENGTH - 1);  
    if (bytes_read > 0) {
        role[bytes_read] = '\0';  
    }
    strtrim(role);
    role[strcspn(role, "\n")] = '\0';  

    
    int fd = open("employees.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
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

    
    hash_password(password, hashed_password);

    
    snprintf(buffer, sizeof(buffer), "%s,%s,%s\n", username, hashed_password, role);
    write(fd, buffer, strlen(buffer));  

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    
    char loans_filename[110];
    snprintf(loans_filename, sizeof(loans_filename), "%s_loans.txt", username);
    int loans_fd = open(loans_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (loans_fd < 0) {
        write(client_sock, "Error: Cannot create loans file for the employee.\n", 51);
        return;
    }
    close(loans_fd);  

    
}


void modify_user_details(int client_sock) {
    char target_username[MAX_LENGTH], new_username[MAX_LENGTH], new_password[MAX_LENGTH], new_role[MAX_LENGTH];
    char line[1024], temp_line[1024];
    char current_username[MAX_LENGTH], current_password[MAX_LENGTH], current_role[MAX_LENGTH], hashed_password[64];

    
    write(client_sock, "Enter username to modify: ", 25);
    int bytes_read = read(client_sock, target_username, MAX_LENGTH);
    target_username[bytes_read] = '\0';  
    strtrim(target_username);
    target_username[strcspn(target_username, "\n")] = '\0';  

    write(client_sock, "Enter new username (or press Enter to skip): ", 44);
    bytes_read = read(client_sock, new_username, MAX_LENGTH);
    new_username[bytes_read] = '\0';  
    strtrim(new_username);
    new_username[strcspn(new_username, "\n")] = '\0';  

    write(client_sock, "Enter new password (or press Enter to skip): ", 44);
    bytes_read = read(client_sock, new_password, MAX_LENGTH);
    new_password[bytes_read] = '\0';  
    strtrim(new_password);
    new_password[strcspn(new_password, "\n")] = '\0';  

    write(client_sock, "Enter new role (or press Enter to skip): ", 42);
    bytes_read = read(client_sock, new_role, MAX_LENGTH);
    new_role[bytes_read] = '\0';  
    strtrim(new_role);
    new_role[strcspn(new_role, "\n")] = '\0';  

    
    int fd = open("employees.txt", O_RDWR);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
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

    
    int temp_fd = open("temp_employees.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        write(client_sock, "Error: Cannot create temp file.\n", 33);
        close(fd);
        return;
    }

    
    ssize_t bytes_read_file;
    char *line_ptr;
    while ((bytes_read_file = read(fd, line, sizeof(line) - 1)) > 0) {
        line[bytes_read_file] = '\0';  
        line_ptr = line;

        char *newline_ptr;
        
        while ((newline_ptr = strchr(line_ptr, '\n')) != NULL) {
            *newline_ptr = '\0';  

            
            sscanf(line_ptr, "%[^,],%[^,],%s\n", current_username, current_password, current_role);
            strtrim(current_username);  

            
            if (strcmp(current_username, target_username) == 0) {
                
                if (strlen(new_username) > 0) strcpy(current_username, new_username);
                if (strlen(new_password) > 0) {
                    hash_password(new_password, hashed_password);  
                    strcpy(current_password, hashed_password);
                }
                if (strlen(new_role) > 0) strcpy(current_role, new_role);

                
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

                
                snprintf(temp_line, sizeof(temp_line), "%s,%s,%s\n", current_username, current_password, current_role);
            } else {
                
                strncpy(temp_line, line_ptr, sizeof(temp_line));
                strncat(temp_line, "\n", sizeof(temp_line) - strlen(temp_line) - 1);  
            }

            
            write(temp_fd, temp_line, strlen(temp_line));

            
            line_ptr = newline_ptr + 1;
        }
    }

    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    close(temp_fd);

    
    rename("temp_employees.txt", "employees.txt");

    
    
}





void manage_user_roles(int client_sock) {
    char buffer[1024];
    int fd = open("employees.txt", O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Error: Cannot open employees file.\n", 35);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;  
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        write(client_sock, "Error: Could not lock file.\n", 28);
        close(fd);
        return;
    }

    
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  
        write(client_sock, buffer, bytes_read);
    }

    
    const char* end_signal = "type ok\0";
    write(client_sock, end_signal, strlen(end_signal));
    read(client_sock,buffer,sizeof(buffer));

    lock.l_type = F_UNLCK;  
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
}






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

