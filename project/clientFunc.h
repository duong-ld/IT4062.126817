#ifndef _CLIENT_FUNC_H
#define _CLIENT_FUNC_H

int getUserChoice(int);
void getString(char*, char*);
int extractServerMessage(char*, int);
void createMessage(char*, int, char*, char*, char*);

int login(int, int);
int signup(int, int);
int logout(int, int);

#endif  // _CLIENT_FUNC_H
