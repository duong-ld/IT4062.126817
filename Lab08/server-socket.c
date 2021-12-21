#include <mysql/mysql.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

MYSQL* con;

// Remember to use -pthread when compiling this server's source code
void* connection_handler(void*);
int convert_message(char*);
void finish_with_error(MYSQL* con);

int main() {
  con = mysql_init(NULL);
  if (con == NULL) {
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
  }

  if (mysql_real_connect(con, "localhost", "user12", "34klq*", "testdb", 0,
                         NULL, 0) == NULL) {
    finish_with_error(con);
  }

  char server_message[100] = "Hello from Server!!\n";
  int server_socket;
  int PORT = 5550;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    perror("Socket initialisation failed");
    exit(EXIT_FAILURE);
  } else
    printf("Server socket created successfully\n");

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // bind the socket to the specified IP addr and port
  if (bind(server_socket, (struct sockaddr*)&server_addr,
           sizeof(server_addr)) != 0) {
    printf("socket bind failed...\n");
    exit(0);
  } else
    printf("Socket successfully binded..\n");

  if (listen(server_socket, 3) != 0) {
    printf("Listen failed...\n");
    exit(0);
  } else
    printf("Server listening..\n");

  int no_threads = 0;
  pthread_t threads[3];
  while (no_threads < 3) {
    printf("Listening...\n");
    int client_socket = accept(server_socket, NULL, NULL);
    puts("Connection accepted");
    if (pthread_create(&threads[no_threads], NULL, connection_handler,
                       &client_socket) < 0) {
      perror("Could not create thread");
      return 1;
    }
    if (client_socket < 0) {
      printf("server acccept failed...\n");
      exit(0);
    } else
      printf("Server acccept the client...\n");
    puts("Handler assigned");
    no_threads++;
  }
  int k = 0;
  for (k = 0; k < 3; k++) {
    pthread_join(threads[k], NULL);
  }

  // int send_status;
  // send_status=send(client_socket, server_message, sizeof(server_message), 0);
  close(server_socket);

  return 0;
}

void* connection_handler(void* client_socket) {
  int socket = *(int*)client_socket;
  int read_len = 0;
  char server_message[100] = "Hello from server\n";
  int send_status = 0;
  send_status = send(socket, server_message, sizeof(server_message), 0);

  char client_message[100] = "\0";
  while ((read_len = recv(socket, client_message, 100, 0)) > 0) {
    client_message[read_len] = '\0';
    if (strcmp(client_message, "q") == 0 || strcmp(client_message, "Q") == 0) {
      printf("Client disconnected\n");
      break;
    }

    int check = convert_message(client_message);
    if (check == 1)
      send_status = send(socket, client_message, strlen(client_message), 0);
    else
      send_status =
          send(socket, "Wrong text format\0", strlen("Wrong text format\0"), 0);

    memset(client_message, 0, sizeof(client_message));
  }

  return 0;
}

int convert_message(char* message) {
  int check = 1;
  for (int i = 0; i < strlen(message); i++) {
    if ('a' <= message[i] && message[i] <= 'z') {
      message[i] = message[i] - 'a' + 'A';
    } else if ('A' <= message[i] && message[i] <= 'Z') {
      continue;
    } else {
      check = 0;
    }
  }
  return check;
}

void finish_with_error(MYSQL* con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}