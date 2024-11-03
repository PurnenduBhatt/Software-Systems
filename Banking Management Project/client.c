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

    
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  
        return;

    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    
    *(end + 1) = '\0';
}



int read_write(int sock, char *buffer) {
    
    bzero(buffer, BUFFER_SIZE); 
    usleep(100000); 

    
    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1); 
    if (bytes_read <= 0) {
        printf("Failed to read from server or server closed the connection.\n");
        return -1; 
    }

    
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);

    
    if (strstr(buffer, "Logging out...") != NULL) {
        return 1; 
    }

    
    printf("Enter your choice:\n");

    
    bzero(buffer, BUFFER_SIZE); 
    usleep(100000); 
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        perror("Error reading input");
        return -1; 
    }

    
    buffer[strcspn(buffer, "\n")] = '\0';

    
    if (write(sock, buffer, strlen(buffer)) <= 0) {
        printf("Failed to send input to server.\n");
        return -1; 
    }

    return 0; 
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    while (1) {
        
        int status = read_write(sock, buffer);
        if (status == -1) break; 
        if (status == 1) break; 

        if (strcmp(buffer, "customer") == 0) {
            
            status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 

            
            status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 
            
            
            bzero(buffer, BUFFER_SIZE); 
            usleep(100000); 

            
            while (1) {
                
                status = read_write(sock, buffer);
                if (status == -1) break; 
                if (status == 1) break; 

                
                if (strcmp(buffer, "1") == 0 || strcmp(buffer, "8") == 0) {
                    
                    bzero(buffer, BUFFER_SIZE); 
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';  
                        printf("Server Response: %s\n", buffer);  
                    } else {
                        printf("Failed to read from server.\n");
                        break;
                    }
                } else {
                    
                     if (strcmp(buffer, "4") == 0 )

                   { status = read_write(sock, buffer);
                    if (status == -1) break; 
                    if (status == 1) break; 
                    status = read_write(sock, buffer);
                    if (status == -1) break; 
                    if (status == 1) break; 
                    
                    }
                    else{
                        status = read_write(sock, buffer);
                    if (status == -1) break; 
                    if (status == 1) break; 
                    
                    }
                }

                
                if (strstr(buffer, "Logging out...") != NULL) {
                    break; 
                }
            }
        }
        else  if (strcmp(buffer, "Administrator") == 0) {
            
               status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 

            
            status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 
            while(1)  {
            

              status = read_write(sock, buffer);
                if (status == -1) break; 
                if (status == 1) break; 

            if (strcmp(buffer, "1") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               
                                bzero(buffer, BUFFER_SIZE);
                usleep(100000);

            }

            
            else if (strcmp(buffer, "2") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
            }

            
            else if (strcmp(buffer, "4") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

              
            }

            else if (strcmp(buffer, "3") == 0) {
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read <= 0) {
            printf("Connection closed or error occurred\n");
            break;  
        }

        buffer[bytes_read] = '\0';  

        strtrim(buffer);

        if (strstr(buffer, "type ok") != NULL) {
            printf("%s\n", buffer); 
            break;
        }

    }
 if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        perror("Error reading input");
        return -1; 
    }

    
    buffer[strcspn(buffer, "\n")] = '\0';

    
    if (write(sock, buffer, strlen(buffer)) <= 0) {
        printf("Failed to send input to server.\n");
        return -1; 
    }
    
    
    
   
}

            }
        }
        else  if (strcmp(buffer, "Employee") == 0) {
            
               status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 

            
            status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 
            while(1)  {
            

              status = read_write(sock, buffer);
                if (status == -1) break; 
                if (status == 1) break; 

            if (strcmp(buffer, "1") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                

               
            }

            
            if (strcmp(buffer, "2") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;



             
            }

            
            if (strcmp(buffer, "3") == 0) {
                
                strtrim(buffer);
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  
                }
            }

            
            if (strcmp(buffer, "5") == 0) {
                
                
                while (1) {
                    bzero(buffer, BUFFER_SIZE);
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    strtrim(buffer);
                    if (bytes_read <= 0)
                     {
                        break;  
                    }
                    
                    
                    if(strstr(buffer,"Type ok")!=NULL);
                    {printf("%s\n", buffer); 
                    break;
                     }
                     buffer[bytes_read] = '\0';
                }
                if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                perror("Error reading input");
                return -1; 
    }

    
    buffer[strcspn(buffer, "\n")] = '\0';

    
    if (write(sock, buffer, strlen(buffer)) <= 0) {
        printf("Failed to send input to server.\n");
        return -1; 
    }
    
            }
             if (strcmp(buffer, "4") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               

               
            }
        }
    }
    else  if (strcmp(buffer, "Manager") == 0) {
            
               status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 

            
            status = read_write(sock, buffer);
            if (status == -1) break; 
            if (status == 1) break; 
            while(1)  {
            

              status = read_write(sock, buffer);
                if (status == -1) break; 
                if (status == 1) break; 

            if (strcmp(buffer, "1") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                


            }

            
            if (strcmp(buffer, "2") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

            }

            
            if (strcmp(buffer, "3") == 0) {
                
                strtrim(buffer);
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  
                }
            }

            
            if (strcmp(buffer, "5") == 0) {
                
                
                while (1) {
                    bzero(buffer, BUFFER_SIZE);
                    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                    strtrim(buffer);
                    if (bytes_read <= 0)
                     {
                        break;  
                    }
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  
                }
            }
             if (strcmp(buffer, "4") == 0) {
                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

                
                status = read_write(sock, buffer);
                if (status == -1 || status == 1) break;

               

                
                bzero(buffer, BUFFER_SIZE);
                int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s\n", buffer);  
                }
            }
        }
    }
   
}
}
