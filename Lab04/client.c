/*UDP Echo Client*/
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serverConstain.h"
#include "serverUtil.h"

void inputMess(char* mess) {
  printf("Insert string to send: ");
  memset(mess, '\0', (strlen(mess) + 1));
  fgets(mess, BUFF_SIZE, stdin);
  mess[strlen(mess) - 1] = '\0';
}

int endOfProcess(char* mess) {
  if (strlen(mess) == 0) {
    printf("\nEnd the process\n");
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s <IP> <Port>\n", argv[0]);
    exit(1);
  }

  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr;
  int bytes_sent, bytes_received, sin_size;
  const int SERV_PORT = atoi(argv[2]);
  char SERV_IP[16] = "\0";
  strcpy(SERV_IP, argv[1]);

  // Step 1: Construct a UDP socket
  if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { /* calls socket() */
    perror("\nError: ");
    exit(0);
  }

  // Step 2: Define the address of the server
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERV_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERV_IP);

  // set role for client
  sin_size = sizeof(struct sockaddr);
  sendMess(SERVER_PASSWORD, &server_addr, client_sock);
  recvMess(buff, &server_addr, client_sock);
  // server is busy
  if (strcmp(buff, SERVER_REFUSED) == 0) {
    printf("\nServer refused connection\n");
    close(client_sock);
    exit(0);
  }

  int role = buff[0] - '0';
  if (role == 1) {
    printf("\nConnected!\nYou can send message when round is odd\n\n");
  } else if (role == 2) {
    printf("\nConnected!\nYou can send message when round is even\n\n");
  }

  // waiting for other client
  printf("Waiting for other client...\n");
  recvMess(buff, &server_addr, client_sock);
  if (strcmp(buff, SERVER_ALLSET) == 0) {
    printf("\nOther client is connected\n");
  } else {
    printf("\nSomething went wrong!!!\n");
    close(client_sock);
    exit(1);
  }

  // Step 3: Communicate with server
  // start round
  int round = 1;

  while (1) {
    printf("\n-----------------------------------\n");
    printf("Round %d: \n", round);
    if ((role + round) % 2 == 0) {
      inputMess(buff);
      sendMess(buff, &server_addr, client_sock);
      if (endOfProcess(buff)) {
        break;
      }
    } else {
      printf("Waiting for message from server...\n");
      recvMess(buff, &server_addr, client_sock);
      if (endOfProcess(buff)) {
        break;
      }
      printf("Message from server:\n%s\n", buff);
    }
    round++;
  }

  close(client_sock);
  return 0;
}
