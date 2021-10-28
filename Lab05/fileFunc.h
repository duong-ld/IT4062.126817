//
//  fileFunc.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _FILEFUNC_H_
#define _FILEFUNC_H_

#include "account.h"
#include "linkedList.h"

Account readAccountFromFile(FILE* fp);
void readListAccountFromFile(char* fileName, Node* head);
void printAccountToFile(FILE* pf, Account account);
void printListAccountToFile(char* fileName, Node head);

#endif /* _FILEFUNC_H_ */
