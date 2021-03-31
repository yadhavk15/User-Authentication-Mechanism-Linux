#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

char *mkrndstr(size_t length){ //Generates random string
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"; 
    char *randomString;
    srand(time(0));
    if (length){
        randomString = malloc(length + 1); 
        if (randomString){
            int l = (int)(sizeof(charset) - 1); 
            int key;                         
            for (int n = 0; n < length; n++){
                key = rand() % l; 
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }
    return randomString;
}

void registration(){

    int fd,flag = 1 ,username_flag = 0;
    fd = open("shadow.txt", O_RDONLY);    
    char ch[2],username[30],bufferregistration[150];
    
    do{
        printf("Enter username: ");
        scanf("%s", username);
        username_flag = 0;
        while (flag > 0){ //check if username already exists in file
            int i = 0;
            while (1){
                flag = read(fd, ch, 1);
                if (flag <= 0){
                    break;
                }
                if (ch[0] == '\n'){
                    char *p = strtok(bufferregistration, ":");///split record 
                    char *array[3];
                    int j = 0;
                    while (p != NULL){
                        array[j++] = p;
                        p = strtok(NULL, ":"); 
                    }
                    if ((strcmp(array[0], username) == 0)){
                        memset(username, 0, strlen(username));//reset  string value
                        printf("Username already exists ,enter a unique username\n");
                        username_flag = 1;
                        break;
                    }
                    break;
                }
                if (i == 0){
                    memset(bufferregistration, 0, strlen(bufferregistration)); //reset  string value each time a new record is read
                    strncpy(bufferregistration, ch, 1);
                    i++;
                }
                else{
                    strncat(bufferregistration, ch, 1);
                }
            }
            if (username_flag == 1){
                break;
            }
        }
    } while (username_flag == 1);
    close(fd);

    char record[130],password[50];
    strcat(username, ":");
    strcat(record, username); //concatenate username to record

    do
    {
        printf("Enter password: ");
        scanf("%s", &password);
        if (strlen(password) < 4){
            printf("Password length is less than 4,enter another\n");
        }
    } while (strlen(password) < 4);

    char *id = "$6$"; //SHA -512
    char salt[19];
    strcat(salt, id);
    char *randomstring = mkrndstr(16);
    strcat(salt, randomstring);

    char *encrypted = crypt(password, salt);
    strcat(encrypted, ":");
    strcat(record, encrypted);//concatenate password to record
    strcat(record, salt);

    fd = open("shadow.txt", O_CREAT | O_WRONLY | O_APPEND, 0777);
    write(fd, record, strlen(record)); // write registered user data to file
    write(fd, "\n", 1);
    close(fd);
    printf("Registration Sucessful\n");
}

void login(){

    char username_entered[30];
    char *password_entered;

    printf("Enter username: ");
    scanf("%s", &username_entered);

    password_entered= getpass("Enter Password");

    int fd,flag = 1,login_flag = 0;
    fd = open("shadow.txt", O_RDONLY);
    char buffer[150];   
    char ch[2];

    while (flag > 0){
        int i = 0;
        while (1){
            flag = read(fd, ch, 1);
            if (flag <= 0){
                break;
            }
            if (ch[0] == '\n'){
                char *p = strtok(buffer, ":");
                char *array[3];
                int j = 0;
                while (p != NULL){
                    array[j++] = p;
                    p = strtok(NULL, ":");
                }
                //array[0]->username    array[1]->encrypted password    array[2]->salt comparing username and encrypted password
                if ((strcmp(array[0], username_entered) == 0) && (strcmp(array[1], crypt(password_entered, array[2])) == 0)){
                    login_flag = 1;
                    break;
                }
                break;
            }
            if (i == 0){
                memset(buffer, 0, strlen(buffer)); //reset  string value each time a new record is read
                strncpy(buffer, ch, 1);
                i++;
            }
            else{
                strncat(buffer, ch, 1);
            }
        }
    }
    if (login_flag != 1){
        printf("Wrong Username or Password entered\n");
    }
    else{
        printf("Login Sucessful\n");
    }
    close(fd);
}

int main(){

    printf("Press 1 to register\n");
    printf("Press 2 to login\n");
    printf("Press 3 to exit\n");

    int choice;
    scanf("%d", &choice);

    switch (choice){
    case 1:
        registration();
        break;
    case 2:
        login();
        break;
    case 3:
        return 0;
    default:
        printf("Error in choice, Re enter!");
        scanf("%d", &choice);
    }
   
    return 0;
}
