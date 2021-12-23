#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5501
#define BUFF_SIZE 1024

int hanldeChoice(int choice, int connfd);

int main() {
  int client_sock;
  int continuer = 1;
  char buff[BUFF_SIZE + 1];
  struct sockaddr_in server_addr; /* server's address information */
  int choice;

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
  // menu
  printf("\n--------------------\n");
  printf("Menu:\n");
  printf("1. Caesar Encrypt File\n");
  printf("2. Caesar Decrypt File\n");
  printf("3. Exit\n");
  printf("--------------------\n");
  printf("Please enter your choice: ");
  scanf("%d", &choice);
  continuer = hanldeChoice(choice, client_sock);

  // Step 4: Close socket
  close(client_sock);
  return 0;
}

int hanldeChoice(int choice, int connfd) {
  Message message;
  FILE* pf = NULL;
  FILE* pf_encrypt = NULL;
  FILE* pf_decrypt = NULL;
  char fileName[BUFF_SIZE] = "\0";

  switch (choice) {
    case 1:
      printf("\nCaesar Encrypt File\n");
      printf("Enter key: ");
      scanf("%s", message.data);
      message.opcode = ENCRYPT_CAESAR;
      sendMessage(connfd, &message);

      printf("Enter file name: ");
      scanf("%s", fileName);
      pf = fopen(fileName, "r");
      sendFile(connfd, pf);
      fclose(pf);

      pf_encrypt = fopen("encrypt_result.txt", "w");
      receiveFile(connfd, pf_encrypt);
      fclose(pf_encrypt);
      break;
    case 2:
      printf("\nCaesar Decrypt File\n");
      printf("Enter key: ");
      scanf("%s", message.data);
      message.opcode = DECRYPT_CAESAR;
      sendMessage(connfd, &message);

      printf("Enter file name: ");
      scanf("%s", fileName);
      pf = fopen(fileName, "r");
      sendFile(connfd, pf);
      fclose(pf);

      pf_decrypt = fopen("decrypt_result.txt", "w");
      receiveFile(connfd, pf_decrypt);
      fclose(pf_decrypt);
      break;
    case 3:
      printf("\nExit\n");
      return 0;
    default:
      printf("Unknown choice: %d\n", choice);
      break;
  }
  return 1;
}