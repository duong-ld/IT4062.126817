#ifndef _SERVERFUNC_H_
#define _SERVERFUNC_H_

#include <stdio.h>
#include <stdlib.h>

void encryptCaesar(FILE *input, FILE *output, int key);
void decryptCaesar(FILE *input, FILE *output, int key);
int handleMessage(Message *message, int connfd);

#endif
