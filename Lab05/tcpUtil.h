
#ifndef _TCP_UTIL_H_
#define _TCP_UTIL_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void sendMess(char* mess, int sockfd);
void recvMess(char* mess, int sockfd);



#endif // _TCP_UTIL_H_


