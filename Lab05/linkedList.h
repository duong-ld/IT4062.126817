//
//  linkedList.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//


#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "account.h"

struct Element {
  Account data;
  struct Element* next;
};

typedef struct Element* Node;

void initList(Node* head);
Node createNode(Account data);
void addHead(Node* head, Account data);
void printList(Node head);
int deleteNode(Node* head, char* username);
Node searchNode(Node head, char* username);
void freeList(Node head);

#endif /* _LINKEDLIST_H_ */


