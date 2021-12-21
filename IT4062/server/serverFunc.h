#ifndef _SERVER_FUNC_H
#define _SERVER_FUNC_H

void hanlde_message(char*, int);
void registerUser(char*, int);
void loginUser(char*, int);
void special_question(char* message, int socket);
void question(char* message, int socket);
void encryptPassword(char*);
void finish_with_error(MYSQL* con);

#endif  // _SERVER_FUNC_H
