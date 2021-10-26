/*UDP Echo Server*/
#include <ctype.h>
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serverConstain.h"
#include "serverUtil.h"

/*
buff = "12h3ll4o"
=> mess = "1234\nhello"
*/
void convertBuffToMess(char* buff, char* mess) {
  int i = 0;
  int j = 0;
  int index = i + j;
  char tmp[BUFF_SIZE] = "\0";
  memset(mess, '\0', (strlen(mess) + 1));

  while (buff[index] != '\0') {
    if (buff[index] >= '0' && buff[index] <= '9') {
      mess[i] = buff[index];
      i++;
      index++;
    } else if ((buff[index] >= 'a' && buff[index] <= 'z') ||
               (buff[index] >= 'A' && buff[index] <= 'Z')) {
      tmp[j] = buff[index];
      j++;
      index++;
    } else {
      memset(mess, '\0', (strlen(mess) + 1));
      strcpy(mess, "Error\n");
      return;
    }
  }
  mess[i] = '\0';
  tmp[j] = '\0';

  if (mess[0] == '\0') {
    strcpy(mess, tmp);
  } else if (tmp[0] == '\0') {
    ;
  } else {
    strcat(mess, "\n");
    strcat(mess, tmp);
    mess[strlen(mess)] = '\0';
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int server_sock; /* file descriptors */
  char buff[BUFF_SIZE] = "\0";
  char mess[BUFF_SIZE] = "\0";
  const int PORT = atoi(argv[1]);

  int bytes_sent, bytes_received;
  struct sockaddr_in server;              /* server's address information */
  struct sockaddr_in client[MAX_CLIENTS]; /* client's address information */
  int sin_size;

  // Step 1: Construct a UDP socket
  if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) ==
      -1) { /* calls socket() */
    perror("\nError: ");
    exit(0);
  }

  // Step 2: Bind address to socket
  server.sin_family = AF_INET;
  server.sin_port =
      htons(PORT); /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr =
      INADDR_ANY; /* INADDR_ANY puts your IP address automatically */
  bzero(&(server.sin_zero), 8); /* zero the rest of the structure */

  if (bind(server_sock, (struct sockaddr*)&server, sizeof(struct sockaddr)) ==
      -1) { /* calls bind() */
    perror("\nError: ");
    exit(0);
  }

// set role for client
SET_ROLE:
  for (int i = 0; i < MAX_CLIENTS; i++) {
    printf("Role %d: Waiting for a client to connect...", i + 1);
    printf("\n");
    recvMess(buff, &client[i], server_sock);
    sprintf(mess, "%d", i + 1);
    sendMess(mess, &client[i], server_sock);
    printf("Role %d: Client connected.\n\n", i + 1);
  }
  // send mess all set
  for (int i = 0; i < MAX_CLIENTS; i++) {
    sendMess(SERVER_ALLSET, &client[i], server_sock);
  }

  // Step 3: Communicate with clients
  // start round
  int round = 1;

  while (1) {
    recvMess(buff, &client[(round + 1) % 2], server_sock);
    // another client connect to server when communicating
    if (strcmp(buff, SERVER_PASSWORD) == 0) {
      sendMess(SERVER_REFUSED, &client[(round + 1) % 2], server_sock);
      continue;
    }
    convertBuffToMess(buff, mess);
    sendMess(mess, &client[round % 2], server_sock);
    if (strlen(mess) == 0) {
      printf("\nServer: Client disconnected.\n");
      printf("\n-----------------------------------\n\n");
      goto SET_ROLE;
    }
    round++;
  }

  close(server_sock);
  return 0;
}
