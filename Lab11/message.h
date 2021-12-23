#ifndef _MESSSAGE_H_
#define _MESSSAGE_H_

#define BUFF_SIZE 1024

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * @brief Message structure
 * Opcode: 1 byte (0: ENCRYPT_CAESAR, 1: DECRYPT_CAESAR, 2: SEND_FILE, 3: ERROR)
 * Length: 2 bytes
 * Data: n bytes
 */
typedef struct message {
  int opcode;
  int length;
  char data[BUFF_SIZE];
} Message;

enum { ENCRYPT_CAESAR = 0, DECRYPT_CAESAR = 1, SEND_FILE = 2, ERROR = 3 };

int receiveMessage(int connfd, Message* message);
int sendMessage(int connfd, Message* message);
void receiveFile(int connfd, FILE* output);
void sendFile(int connfd, FILE* input);

#endif
