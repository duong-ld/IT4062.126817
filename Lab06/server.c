#include <arpa/inet.h>
#include <libgen.h>
#include <netinet/in.h>
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 2 /* Number of allowed connections */
#define BUFF_SIZE 1024

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

void recvFile(int sockfd, char* filename) {
  FILE* fp;
  char buffer[BUFF_SIZE];
  int n;

  fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("Error opening file!\n");
    return;
  }

  while ((n = recv(sockfd, buffer, BUFF_SIZE, 0)) > 0) {
    fwrite(buffer, sizeof(char), n, fp);
  }
  fclose(fp);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int PORT = atoi(argv[1]);

  int listen_sock, conn_sock; /* file descriptors */
  char recv_data[BUFF_SIZE];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  int sin_size;

  // Step 1: Construct a TCP socket to listen connection request
  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) ==
      -1) { /* calls socket() */
    perror("\nError: ");
    return 0;
  }

  // Step 2: Bind address to socket
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port =
      htons(PORT); /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr =
      htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
  if (bind(listen_sock, (struct sockaddr*)&server, sizeof(server)) ==
      -1) { /* calls bind() */
    perror("\nError: ");
    return 0;
  }

  // Step 3: Listen request from client
  if (listen(listen_sock, BACKLOG) == -1) { /* calls listen() */
    perror("\nError: ");
    return 0;
  }

  // Step 4: Communicate with client
  while (1) {
		printf("Waiting for connection...\n");
    // accept request
    sin_size = sizeof(struct sockaddr_in);
    if ((conn_sock = accept(listen_sock, (struct sockaddr*)&client,
                            (socklen_t*)&sin_size)) == -1)
      perror("\nError: ");

    printf("You got a connection from %s\n",
           inet_ntoa(client.sin_addr)); /* prints client's IP */

    // receive file name
    bytes_received = recv(conn_sock, recv_data, BUFF_SIZE, 0);

    if (bytes_received == 0) {
      close(conn_sock);
      printf("Client disconnected\n");
			printf("\n--------------------------------\n");
      continue;
    }

    recv_data[bytes_received] = '\0';
    printf("Received file name: %s\n", recv_data);
    normalizeFile(recv_data);
    char* filename = basename(recv_data);
    filename[strlen(filename)] = '\0';
    char path[BUFF_SIZE] = "recv/";
    strcat(path, filename);
    printf("Converted path: %s\n", path);

    // check file_name is exist
    FILE* pf = fopen(path, "r");
    if (pf != NULL) {
      send(conn_sock, "0", 1, 0);
      fclose(pf);
      close(conn_sock);
      printf("Client disconnected\n");
			printf("\n--------------------------------\n");
      continue;
    } else {
      send(conn_sock, "1", 1, 0);
    }

    // receive file
    recvFile(conn_sock, path);
    printf("Received file: %s\n", filename);

    // close connection
    close(conn_sock);
    printf("Client disconnected\n");
		printf("\n--------------------------------\n");
  }

  close(listen_sock);
  return 0;
}
