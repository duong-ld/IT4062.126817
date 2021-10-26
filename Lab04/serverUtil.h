#ifndef _SERVERFUNC_H_
#define _SERVERFUNC_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void sendMess(char* mess, struct sockaddr_in* addr, int sockfd);
void recvMess(char* mess, struct sockaddr_in* addr, int sockfd);

#endif  // _SERVERFUNC_H_