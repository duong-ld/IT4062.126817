#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {
  // create a socket
  int network_socket;
  int PORT = 5550;
  network_socket = socket(AF_INET, SOCK_STREAM, 0);

  // specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int connection_status =
      connect(network_socket, (struct sockaddr*)&server_address,
              sizeof(server_address));
  // check for connection_status
  if (connection_status == -1) {
    printf("The connection has error\n\n");
  }

  if (connection_status == 0) {
    char response[256] = "\0";
    // receive data from the server
    recv(network_socket, &response, sizeof(response), 0);
    printf("%s\n", response);
    // total data sent to the server
    int total_sent = 0;

    while (1) {
      printf("enter a message to echo\n");
      scanf("%[^\n]s", response);
      getchar();

      int send_status = send(network_socket, response, strlen(response), 0);
      total_sent += send_status;

      if (strcmp(response, "q") == 0 || strcmp(response, "Q") == 0) {
        printf("The total data sent to the server is %d\n\n", total_sent);
        break;
      }

      memset(response, 0, sizeof(response));

      int recv_status = recv(network_socket, &response, sizeof(response), 0);
      response[recv_status] = '\0';
      // print out the server's response
      printf("Here is the echo message from the server: %s\n\n", response);
    }
  }
  // close the socket
  close(network_socket);

  return 0;
}
