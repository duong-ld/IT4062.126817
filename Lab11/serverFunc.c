#include <stdio.h>
#include <stdlib.h>

#include "message.h"
#include "serverFunc.h"

void encryptCaesar(FILE* input, FILE* output, int key) {
  char c;
  while ((c = fgetc(input)) != EOF) {
    if (c >= 'a' && c <= 'z') {
      c = c + key;
      if (c > 'z') {
        c = c - 'z' + 'a' - 1;
      }
    } else if (c >= 'A' && c <= 'Z') {
      c = c + key;
      if (c > 'Z') {
        c = c - 'Z' + 'A' - 1;
      }
    }
    fputc(c, output);
  }
}

void decryptCaesar(FILE* input, FILE* output, int key) {
  char c;
  while ((c = fgetc(input)) != EOF) {
    if (c >= 'a' && c <= 'z') {
      c = c - key;
      if (c < 'a') {
        c = c + 'z' - 'a' + 1;
      }
    } else if (c >= 'A' && c <= 'Z') {
      c = c - key;
      if (c < 'A') {
        c = c + 'Z' - 'A' + 1;
      }
    }
    fputc(c, output);
  }
}

int handleMessage(Message* message, int connfd) {
  int key = 0;
  FILE* tmp = NULL;
  FILE* tmp_encrypt = NULL;
  FILE* tmp_decrypt = NULL;

  switch (message->opcode) {
    case ENCRYPT_CAESAR:

      key = atoi(message->data);
      tmp = fopen("tmp.txt", "w");
      receiveFile(connfd, tmp);
      fclose(tmp);

      tmp = fopen("tmp.txt", "r");
      tmp_encrypt = fopen("tmp_ecrypt.txt", "w");
      encryptCaesar(tmp, tmp_encrypt, key);
      fclose(tmp);
      fclose(tmp_encrypt);

      tmp_encrypt = fopen("tmp_ecrypt.txt", "r");
      sendFile(connfd, tmp_encrypt);
      fclose(tmp_encrypt);

      break;
    case DECRYPT_CAESAR:

      key = atoi(message->data);
      tmp = fopen("tmp.txt", "w");
      receiveFile(connfd, tmp);
      fclose(tmp);
      
      tmp = fopen("tmp.txt", "r");
      tmp_decrypt = fopen("tmp_decrypt.txt", "w");
      decryptCaesar(tmp, tmp_decrypt, key);
      fclose(tmp);
      fclose(tmp_decrypt);

      tmp_decrypt = fopen("tmp_decrypt.txt", "r");
      sendFile(connfd, tmp_decrypt);
      fclose(tmp_decrypt);
      
      break;
    default:
      printf("Unknown message type: %d\n", message->opcode);
      break;
  }
}
