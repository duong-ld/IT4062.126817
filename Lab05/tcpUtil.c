#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcpUtil.h"

void sendMess(char* mess, int sockfd) {
  int bytes_sent = send(sockfd, mess, strlen(mess), 0);
  if (bytes_sent <= 0) {
    printf("\nConnection closed!\n");
    close(sockfd);
    exit(1);
  }
}
void recvMess(char* mess, int sockfd) {
  char buff[1024] = "\0";
  socklen_t plen = sizeof(struct sockaddr_in);
  int bytes_received = recv(sockfd, buff, 1024, 0);
  if (bytes_received < 0) {
    perror("ERROR in recvfrom");
    close(sockfd);
    exit(1);
  }

  buff[bytes_received] = '\0';
  memset(mess, '\0', (strlen(mess) + 1));
  strcpy(mess, buff);
  mess[strlen(mess)] = '\0';
}