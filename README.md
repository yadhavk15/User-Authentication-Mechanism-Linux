# User Authentication Mechanism Linux
A C program which simulates the User Authentication Mechanism in Linux

# How to run

```bash
gcc -o test  Mechanism.c -lcrypt
```
#Solution Definition

## registration()

- Enter username and password(length >4).
- The file shadow.txt  is opened to compare the username you entered with the ones in the file to check if it exists. If so, re-enter a unique username.
- The mkrnstr() function takes as parameter the length of the random salt you want it to generate and return it. In this case, we are using a 19 character salt. 
- Then an id is concatenated to the salt which indicates which encryption algorithm is being used. 
- $6$ as id means that the SHA-512 encryption method is being used as per man(http://man7.org/linux/man-pages/man3/crypt.3.html).
- Then the password is encrypted using the crypt() function from the  <crypt.h> library. It takes as parameters the password entered and the salt. 
- The username, password, and salt are concatenated into a single string (record) and are separated by the “:” character. Then it is saved in shadow.txt file.

## login()

- Enter username and password(upon input password is hidden in the terminal using getpass() instead of scanf()).
- The shadow.txt file is read to see if such a username exists. 
- If the username exists, the encrypted password and salt are read from that row. strtok() is used to split the row into an array of strings(separated by “:”). 
- The password entered is encrypted using the salt from that row and then it is compared with the one in the file.
- A login successful message is displayed if the comparison returns true.
- Else it returns an error message which does not tell you specifically whether the username or password or both were wrongly entered. This is for security purposes.
