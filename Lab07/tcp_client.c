#include <arpa/inet.h>
#include <libgen.h>
#include <netinet/in.h>
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 1024

// send file .txt to server in text format
int sendAndRecvFile(int sockfd, char* fileName, char* recvFile) {
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  int bytes_sent = 0;
  int bytes_recv = 0;
  int total_bytes_sent = 0;

  FILE* fp_send = fopen(fileName, "r");
  if (fp_send == NULL) {
    printf("File not found\n");
    exit(EXIT_FAILURE);
  }
  FILE* fp_recv = fopen(recvFile, "w");
  if (fp_recv == NULL) {
    printf("File not found\n");
    exit(EXIT_FAILURE);
  }

  printf("\nReceived File Content: \n");
  while ((read = getline(&line, &len, fp_send)) != -1) {
    // send file to server
    bytes_sent = send(sockfd, line, strlen(line), 0);
    if (bytes_sent < 0) {
      printf("Send failed\n");
      exit(EXIT_FAILURE);
    }

    // recv file from server
    bytes_recv = recv(sockfd, line, BUFF_SIZE, 0);
    if (bytes_recv < 0) {
      printf("Recv failed\n");
      exit(EXIT_FAILURE);
    }

    // print file content
    printf("%s", line);
    fprintf(fp_recv, "%s", line);

    // add bytes sent to total bytes sent
    total_bytes_sent += bytes_sent;
  }
  fclose(fp_send);
  fclose(fp_recv);
  return total_bytes_sent;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Usage: %s <server_address> <port> <file_name>\n", argv[0]);
		exit(EXIT_FAILURE);
  }
  char* SERVER_ADDR = argv[1];
  int SERVER_PORT = atoi(argv[2]);
  char* fileName = argv[3];

  int client_sock;
  char buff[BUFF_SIZE + 1];
  struct sockaddr_in server_addr; /* server's address information */
  int msg_len, bytes_sent, bytes_received;

  // Step 1: Construct socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);

  // Step 2: Specify server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  // Step 3: Request to connect server
  if (connect(client_sock, (struct sockaddr*)&server_addr,
              sizeof(struct sockaddr)) < 0) {
    printf("\nError!Can not connect to sever! Client exit imediately! ");
    return 0;
  }

  // Step 4: Communicate with server
  // send file name
  bytes_sent = send(client_sock, fileName, strlen(fileName), 0);
  if (bytes_sent < 0) {
    printf("Send failed\n");
    exit(EXIT_FAILURE);
  }
  // recv file name
  bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    printf("Recv failed\n");
    exit(EXIT_FAILURE);
  }
  buff[bytes_received] = '\0';
  printf("Received File Name: %s\n", buff);
  // send file content to server and recv file content from server
  int bytes_sent_file = sendAndRecvFile(client_sock, fileName, buff);
  printf("\n\n");
  // print number of bytes sent
  printf("Bytes sent (with out file name): %d\n", bytes_sent_file);
  printf("Bytes sent (with file name): %d\n", bytes_sent + bytes_sent_file);
  close(client_sock);
  return 0;
}
