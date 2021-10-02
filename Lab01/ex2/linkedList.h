//
//  linkedList.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "studentData.h"

struct Element {
  StudentData *data;
  struct Element* next;
};
typedef struct Element* Node;

void createList(Node* head);
Node createNode(StudentData *data);
void addHead(Node* head, StudentData *data);
void printList(Node head);
int deleteNode(Node* head, char* studentID);
StudentData* searchNode(Node head, char* studentID);
void freeList(Node head);

#endif /* _LINKEDLIST_H_ */


