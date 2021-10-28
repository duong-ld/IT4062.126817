#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "tcpUtil.h"

#define BUFF_SIZE 1024

#define INACTIVE 0
#define LOGIN_SUCCESS 1
#define LOGIN_FAILED 2
#define BLOCKED 3
#define DISCONNECT "4"

void inputMess(char* mess) {
  memset(mess, '\0', (strlen(mess) + 1));
  fgets(mess, BUFF_SIZE, stdin);
  mess[strlen(mess) - 1] = '\0';
}

int main(int argc, char* argv[]) {
  char username[BUFF_SIZE] = "\0";
  char password[BUFF_SIZE] = "\0";
  int status = 0;
  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    return 1;
  }
  int SERVER_PORT = atoi(argv[2]);
  char SERVER_ADDR[BUFF_SIZE] = "\0";
  strcpy(SERVER_ADDR, argv[1]);
  SERVER_ADDR[strlen(SERVER_ADDR)] = '\0';

  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr; /* server's address information */
  int msg_len, bytes_sent, bytes_received;

  // Step 1: Construct socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);

  // Step 2: Specify server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  // Step 3: Request to connect server
  if (connect(client_sock, (struct sockaddr*)&server_addr,
              sizeof(struct sockaddr)) < 0) {
    printf("\nError!Can not connect to sever! Client exit imediately! ");
    return 0;
  }

  // Step 4: Communicate with server
  do {
    // enter username and password
    printf("\nEnter username: ");
    inputMess(username);
    printf("\nPlease enter the login password: ");
    inputMess(password);

    // send username and password to server
    memset(buff, '\0', BUFF_SIZE);
    strcpy(buff, username);
    strcat(buff, " ");
    strcat(buff, password);
    buff[strlen(buff)] = '\0';

    sendMess(buff, client_sock);

    // receive echo reply
    recvMess(buff, client_sock);
    status = buff[0] - '0';

    if (status == LOGIN_SUCCESS) {
      printf("\nLogin is successful!\n");
      break;
    } else if (status == LOGIN_FAILED) {
      printf("\nPassword is not correct. Please try again!\n");
    } else if (status == BLOCKED) {
      printf("\nAccount is blocked!\n");
      goto EXIT;
    } else {
      printf("\nAccount is blocked or inactive\n");
      goto EXIT;
    }
  } while (1);

  // log out
  do {
    printf("Enter \"Bye\" to bye: ");
    inputMess(buff);
  } while (strcmp(buff, "Bye"));
  printf("Goodbye %s\n", username);
EXIT:
  sendMess(DISCONNECT, client_sock);
  close(client_sock);
  return 0;
}
