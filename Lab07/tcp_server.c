#include <arpa/inet.h>
#include <errno.h>
#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG 20
#define BUFF_SIZE 1024

/* Handler process signal*/
void sig_chld(int signo);

/*
 * Receive and echo message to client
 * [IN] sockfd: socket descriptor that connects to client
 */
void echo(int sockfd);
void toUpperCase(char* str);
void recvAndSendFile(int sockfd);
void normalizeFile(char* filename);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}
	int PORT = atoi(argv[1]);

	
  int listen_sock, conn_sock; /* file descriptors */
  struct sockaddr_in server;  /* server's address information */
  struct sockaddr_in client;  /* client's address information */
  pid_t pid;
  int sin_size;

  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) ==
      -1) { /* calls socket() */
    printf("socket() error\n");
    return 0;
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr =
      htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */

  if (bind(listen_sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
    perror("\nError: ");
    return 0;
  }

  if (listen(listen_sock, BACKLOG) == -1) {
    perror("\nError: ");
    return 0;
  }

  /* Establish a signal handler to catch SIGCHLD */
  signal(SIGCHLD, sig_chld);

  while (1) {
    sin_size = sizeof(struct sockaddr_in);
    if ((conn_sock = accept(listen_sock, (struct sockaddr*)&client,
                            (socklen_t*)&sin_size)) == -1) {
      if (errno == EINTR)
        continue;
      else {
        perror("\nError: ");
        return 0;
      }
    }
    /* For each client, fork spawns a child, and the child handles the new
     * client */
    pid = fork();

    /* fork() is called in child process */
    if (pid == 0) {
      close(listen_sock);
      printf("You got a connection from %s\n",
             inet_ntoa(client.sin_addr)); /* prints client's IP */
      echo(conn_sock);
      exit(0);
    }

    /* The parent closes the connected socket since the child handles the new
     * client */
    close(conn_sock);
  }
  close(listen_sock);
  return 0;
}

void sig_chld(int signo) {
  pid_t pid;
  int stat;

  /* Wait the child process terminate */
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("\nChild %d terminated\n", pid);
}

// recv file from client -> convert to uppercase -> send to client
void echo(int sockfd) {
  char buff[BUFF_SIZE];
  int bytes_sent, bytes_received;

  // recv file name
  bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    perror("recv: ");
    return;
  }

  buff[bytes_received] = '\0';
  normalizeFile(buff);
  char* fileName = basename(buff);
  fileName[strlen(fileName)] = '\0';

  // convert file name to uppercase and send to client
  toUpperCase(fileName);
  bytes_sent = send(sockfd, fileName, strlen(fileName), 0);
  if (bytes_sent < 0) {
    perror("send: ");
    return;
  }
  // recv file from client and send to client
  recvAndSendFile(sockfd);
  close(sockfd);
}

// recv file from client in text format and convert to upper case
void recvAndSendFile(int sockfd) {
  int bytes_sent, bytes_received;
  char buff[BUFF_SIZE];
  while (1) {
    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
    if (bytes_received < 0) {
      perror("recv");
      return;
    } else if (bytes_received == 0) {
      printf("\nClient disconnected\n");
      break;
    }
    buff[bytes_received] = '\0';
    toUpperCase(buff);
    bytes_sent = send(sockfd, buff, strlen(buff), 0);
    if (bytes_sent < 0) {
      perror("\nError: ");
      return;
    }
  }
}

// convert string to upper case
void toUpperCase(char* str) {
  int i;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] >= 'a' && str[i] <= 'z')
      str[i] -= 32;
    if (str[i] == '\n') {
      str[i + 1] = '\0';
      break;
    }
  }
}

void normalizeFile(char* filename) {
  for (int i = 0; i < strlen(filename); i++) {
    if ((filename[i] >= 'a' && filename[i] <= 'z') ||
        (filename[i] >= 'A' && filename[i] <= 'Z') || filename[i] == '.' ||
        filename[i] == '/') {
      continue;
    } else {
      filename[i] = '\0';
      break;
    }
  }
}