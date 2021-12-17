#include <mysql/mysql.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "constain.h"
#include "serverFunc.h"

extern MYSQL* con;

void finish_with_error(MYSQL* con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void hanlde_message(char* message, int socket) {
  int type = message[0] - '0';
  char server_message[100] = "\0";
  switch (type) {
    case LOGIN:
      printf("LOGIN\n");
      loginUser(message, socket);
      break;
    case REGISTER:
      registerUser(message, socket);
      break;
    case LOGOUT:
      break;
    default:
      break;
  }
}

void registerUser(char* message, int socket) {
  char username[100] = "\0";
  char password[100] = "\0";
  char serverMess[100] = "\0";
  char name[100] = "\0";
  char* token;
  token = strtok(message, "|");
  token = strtok(NULL, "|");
  strcpy(name, token);
  token = strtok(NULL, "|");
  strcpy(username, token);
  token = strtok(NULL, "|");
  strcpy(password, token);
  encryptPassword(password);

  char query[200] = "\0";

  sprintf(
      query,
      "INSERT INTO users (name, username, password) VALUES ('%s', '%s', '%s')",
      name, username, password);

  if (mysql_query(con, query)) {
    sprintf(serverMess, "%d|F|%s\n", REGISTER, mysql_error(con));
    send(socket, serverMess, strlen(serverMess), 0);
    return;
  }
  char server_message[100] = "\0";
  sprintf(server_message, "%d|S|Successfully registered %s\n", REGISTER, name);
  send(socket, server_message, sizeof(server_message), 0);
  return;
}

void loginUser(char* message, int socket) {
  char username[100] = "\0";
  char password[100] = "\0";
  char serverMess[100] = "\0";
  char* token;
  token = strtok(message, "|");
  token = strtok(NULL, "|");
  strcpy(username, token);
  token = strtok(NULL, "|");
  strcpy(password, token);
  encryptPassword(password);

  char query[200] = "\0";

  sprintf(query, "SELECT * from users where username='%s' and password='%s'",
          username, password);
  if (mysql_query(con, query)) {
    sprintf(serverMess, "%d|F|%s\n", LOGIN, mysql_error(con));
    send(socket, serverMess, strlen(serverMess), 0);
    return;
  }
  MYSQL_RES* result = mysql_store_result(con);
  if (mysql_num_rows(result) == 0) {
    sprintf(serverMess, "%d|F|Invalid username or password\n", LOGIN);
    send(socket, serverMess, strlen(serverMess), 0);
    return;
  }
  char server_message[100] = "\0";
  sprintf(server_message, "%d|S|Successfully logged in\n", LOGIN);
  send(socket, server_message, sizeof(server_message), 0);
  printf("%s\n", server_message);
  return;
}

void encryptPassword(char* password) {
  int encrypt = 0;
  for (int i = 0; i < strlen(password); i++) {
    if ((password[i] - i >= '0' && password[i] - i <= '9') ||
        (password[i] - i >= 'a' && password[i] - i <= 'z') ||
        (password[i] - i >= 'A' && password[i] - i <= 'Z'))
      password[i] = password[i] - i;
  }
}