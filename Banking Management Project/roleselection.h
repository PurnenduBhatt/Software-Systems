#ifndef ROLESELECTIONMODULE_H
#define ROLESELECTIONMODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<limits.h>
#include"FileManagementmodule.h"



void customer_menu(int sock, const char *username);
int lock_file(int fd, int lock_type, off_t offset, size_t size);
int unlock_file(int fd, off_t offset, size_t size);
int modify_balance(int sock, const char *username, int amount);
void employee_menu(int client_sock, const char *username);
void manager_menu(int client_sock, const char *username);
void admin_menu(int client_sock, const char *username);
int login_user( char *username,  char *password,  char *role);
void view_balance(int sock, const char *username);
void depositMoney(int sock, const char *username);
void create_session(const char *username);
static void strtrim(char *str);
int is_user_logged_in(const char *username,const char *role);
void log_transaction(const char *username, const char *transaction_type, int amount);
void get_balance(int sock, const char *username);
void withdrawMoney(int sock, const char *username);
void transferfunds(int sock, const char *username);
void apply_for_loan(int sock, const char *username);
void change_password(int client_sock, const char *username,char*role);
void add_feedback(int sock);
void view_transaction_history(int sock, const char *username);
void add_new_customer(int client_sock);
void review_customer_feedback(int client_sock);
void assign_loan_application(int client_sock);
void modify_customer_details(int client_sock);
void process_loan_applications(int client_sock);
void approve_reject_loan(int client_sock);
void view_assigned_loans(int client_sock, const char *username);
void activate_deactivate_customer(int client_sock);

#endif 
