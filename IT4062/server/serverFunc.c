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
    case QUESTION:
      question(message, socket);
      break;
    case SPECIAL_QUESTION:
      special_question(message, socket);
      break;
    case LOGOUT:
      break;
    default:
      break;
  }
}

/**
 * @brief get information of product from database convert to question, send to
 * client
 * @param message message from client
 * @param socket socket of client
 * @return void
 * @author Luong Duong
 *
 */
void question(char* message, int socket) {
  char server_message[100] = "\0";
  char product_name[100] = "\0";
  char manufacturer[100] = "\0";
  double price = 0;
  int quantity = 0;
  char query[100] = "\0";

  // get total product in database
  sprintf(query, "SELECT COUNT(*) FROM products");
  if (mysql_query(con, query)) {
    finish_with_error(con);
  }
  MYSQL_RES* result = mysql_store_result(con);
  if (result == NULL) {
    finish_with_error(con);
  }
  MYSQL_ROW row = mysql_fetch_row(result);
  int total_product = atoi(row[0]);
  // get random product
  int random_product = rand() % total_product;
  sprintf(query, "SELECT * FROM products LIMIT %d, 1", random_product);
  if (mysql_query(con, query)) {
    finish_with_error(con);
  }
  result = mysql_store_result(con);
  if (result == NULL) {
    finish_with_error(con);
  }
  row = mysql_fetch_row(result);
  strcpy(product_name, row[1]);
  strcpy(manufacturer, row[2]);
  price = atof(row[3]);
  quantity = rand() % 10 + 1;
  // convert to question
  sprintf(server_message, "%d|What is the price of %d %s's %s|%f|%f", QUESTION,
          quantity, manufacturer, product_name, price * quantity,
          (price * quantity) * 1.05);
  // send to client
  send(socket, server_message, strlen(server_message), 0);
}

/**
 * @brief get information of list products from database convert to question,
 * send to client
 * @param message message from client
 * @param socket socket of client
 * @return void
 * @author Luong Duong
 *
 */
void special_question(char* message, int socket) {
  char server_message[100] = "\0";
  char product_name[100] = "\0";
  char manufacturer[100] = "\0";
  double price = 0;
  double total_price = 0;
  int quantity = 0;
  char query[100] = "\0";
  int total_product = 0;

  // get total product in database
  sprintf(query, "SELECT COUNT(*) FROM products");
  if (mysql_query(con, query)) {
    finish_with_error(con);
  }
  MYSQL_RES* result = mysql_store_result(con);
  if (result == NULL) {
    finish_with_error(con);
  }
  MYSQL_ROW row = mysql_fetch_row(result);
  total_product = atoi(row[0]);
  // get 5 random product, convert to 1 question
  char question[500] = "For a list of the following products:\n";
  for (int i = 0; i < 5; i++) {
    char temp[100] = "\0";
    int random_product = rand() % total_product;
    sprintf(query, "SELECT * FROM products LIMIT %d, 1", random_product);
    if (mysql_query(con, query)) {
      finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL) {
      finish_with_error(con);
    }
    row = mysql_fetch_row(result);
    strcpy(product_name, row[1]);
    strcpy(manufacturer, row[2]);
    price = atof(row[3]);
    quantity = rand() % 10 + 1;
    total_price += price * quantity;
    // convert to question
    sprintf(temp, " - %d %s's %s\n", quantity, manufacturer, product_name);
    strcat(question, temp);
  }
  strcat(question, "What is the price of all of these products?");
  // convert to question
  sprintf(server_message, "%d|%s|%f", SPECIAL_QUESTION, question, total_price);
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
