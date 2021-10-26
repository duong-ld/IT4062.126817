#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serverConstain.h"
#include "serverUtil.h"

void sendMess(char* mess, struct sockaddr_in* addr, int sockfd) {
  int n = sendto(sockfd, mess, strlen(mess), 0, (struct sockaddr*)addr,
                 sizeof(struct sockaddr_in));
  if (n < 0) {
    perror("ERROR in sendto");
    close(sockfd);
    exit(1);
  }
}

void recvMess(char* mess, struct sockaddr_in* addr, int sockfd) {
  char buff[BUFF_SIZE] = "\0";
  socklen_t plen = sizeof(struct sockaddr_in);
  int n = recvfrom(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr*)addr, &plen);
  if (n < 0) {
    perror("ERROR in recvfrom");
    close(sockfd);
    exit(1);
  }
  buff[n] = '\0';
  memset(mess, '\0', (strlen(mess) + 1));
  strcpy(mess, buff);
  mess[strlen(mess)] = '\0';
}
