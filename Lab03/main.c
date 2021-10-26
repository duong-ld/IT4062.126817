#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// return 1 if the string is a IPv4 address
int isIPAddress(char* str) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == '.') {
      continue;
    }
    if (str[i] < '0' || str[i] > '9') {
      return 0;
    }
  }
  return 1;
}

void printHostNameFromAddr(char* strAddr) {
  struct in_addr addr;
  if (inet_aton(strAddr, &addr)) {
    struct hostent* host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    if (host) {
      printf("Official name: %s\n", host->h_name);
      for (int i = 0; host->h_aliases[i]; i++) {
        if (i == 0) {
          printf("Alias name:\n");
        }
        printf("%s\n", host->h_aliases[i]);
      }
    } else {
      printf("No Information found\n");
    }
  } else {
    printf("No Information found\n");
  }
}

void printAddrFromHostName(char* hostName) {
  struct hostent* host = gethostbyname(hostName);
  if (host) {
    printf("Official IP: ");
    printf("%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    for (int i = 1; host->h_addr_list[i]; i++) {
      if (i == 1) {
        printf("Alias IP:\n");
      }
      printf("%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
  } else {
    printf("No Information found\n");
  }
}

// program convert a address to host name and vice versa
int main(int argc, char const* argv[]) {
  if (argc != 2) {
    printf("Usage: ./resolver parameter\n");
    return 1;
  }
  char strInput[100] = "\0";
  strcpy(strInput, argv[1]);

  if (isIPAddress(strInput)) {
    printHostNameFromAddr(strInput);
  } else {
    printAddrFromHostName(strInput);
  }
  return 0;
}
