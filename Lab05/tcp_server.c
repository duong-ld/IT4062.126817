#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "account.h"
#include "fileFunc.h"
#include "linkedList.h"
#include "tcpUtil.h"

#define BACKLOG 2 /* Number of allowed connections */
#define BUFF_SIZE 1024

#define INACTIVE "0"
#define LOGIN_SUCCESS "1"
#define LOGIN_FAILED "2"
#define BLOCKED "3"
#define DISCONNECT 4

// mess = username password
void decodeMess(char* mess, char* username, char* password) {
  memset(username, '\0', BUFF_SIZE);
  memset(password, '\0', BUFF_SIZE);

  int i = 0;
  while (mess[i] != ' ') {
    username[i] = mess[i];
    i++;
  }
  username[i] = '\0';
  i++;
  int j = 0;
  while (mess[i] != '\0') {
    password[j] = mess[i];
    i++;
    j++;
  }
  password[j] = '\0';
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int PORT = atoi(argv[1]);

  int listen_sock, conn_sock; /* file descriptors */
  char recv_data[BUFF_SIZE];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  int sin_size;

  // Step 1: Construct a TCP socket to listen connection request
  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) ==
      -1) { /* calls socket() */
    perror("\nError: ");
    return 0;
  }

  // Step 2: Bind address to socket
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port =
      htons(PORT); /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr =
      htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
  if (bind(listen_sock, (struct sockaddr*)&server, sizeof(server)) ==
      -1) { /* calls bind() */
    perror("\nError: ");
    return 0;
  }

  // Step 3: Listen request from client
  if (listen(listen_sock, BACKLOG) == -1) { /* calls listen() */
    perror("\nError: ");
    return 0;
  }

  // Step 4: Communicate with client
  while (1) {
    // accept request
    sin_size = sizeof(struct sockaddr_in);
    if ((conn_sock = accept(listen_sock, (struct sockaddr*)&client,
                            (socklen_t*)&sin_size)) == -1)
      perror("\nError: ");

    printf("You got a connection from %s\n",
           inet_ntoa(client.sin_addr)); /* prints client's IP */

    // read list account from file
    Node head = NULL;
    initList(&head);
    readListAccountFromFile("account.txt", &head);
    // start conversation
    while (1) {
      // receives message from client
      char buff[BUFF_SIZE] = "\0";
      recvMess(buff, conn_sock);
      printf("Received: %s\n", buff);

      // check message
      int status = buff[0] - '0';
      if (status == DISCONNECT) {
        printf("Client is disconnected\n");
        break;
      }

      // decode message
      char username[BUFF_SIZE] = "\0";
      char password[BUFF_SIZE] = "\0";
      decodeMess(buff, username, password);

      // check account
      Node tmp = searchNode(head, username);

      // username not valid
      if (tmp == NULL) {
        sendMess(LOGIN_FAILED, conn_sock);
        continue;
      }

      // password not correct
      if (strcmp(tmp->data->password, password) != 0) {
        if (tmp->data->countLogin == 2) {
          sendMess(BLOCKED, conn_sock);
          tmp->data->status = 0;
        } else {
          sendMess(LOGIN_FAILED, conn_sock);
          tmp->data->countLogin++;
        }
        continue;
      }

      // password correct
      if (tmp->data->status == 0) {
        sendMess(INACTIVE, conn_sock);
      } else {
        sendMess(LOGIN_SUCCESS, conn_sock);
      }
    }  // end conversation

    printListAccountToFile("account.txt", head);
    freeList(head);
    close(conn_sock);
  }
  close(listen_sock);
  return 0;
}
