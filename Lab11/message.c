#include <stdio.h>
#include <stdlib.h>

#include "message.h"

int receiveMessage(int connfd, Message* message) {
  int ret;
  ret = recv(connfd, message, sizeof(Message), 0);
  if (ret == -1) {
    perror("\nError: ");
    return 0;
  }
  return 1;
}

int sendMessage(int connfd, Message* message) {
  int ret;
  ret = send(connfd, message, sizeof(Message), 0);
  if (ret == -1) {
    perror("\nError: ");
    return 0;
  }
  return 1;
}

void receiveFile(int connfd, FILE* output) {
  Message message;

  while (1) {
    if (!receiveMessage(connfd, &message)) {
      break;
    }
    if (message.opcode != SEND_FILE) {
      break;
    }
    if (message.length == 0) {
      break;
    }
    fwrite(message.data, sizeof(char), message.length, output);
  }
}

void sendFile(int connfd, FILE* input) {
  Message message;
  int n;
  message.opcode = SEND_FILE;

  while (1) {
    n = fread(message.data, sizeof(char), BUFF_SIZE - 1, input);
    if (n == 0) {
      break;
    }
    message.length = n;
    if (!sendMessage(connfd, &message)) {
      break;
    }
  }

  message.length = 0;
  if (!sendMessage(connfd, &message)) {
    return;
  }
}