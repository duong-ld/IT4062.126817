//
//  linkedList.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "student.h"

struct Element {
  Student data;
  struct Element* next;
};
typedef struct Element* Node;

void initList(Node* head);
Node createNode(Student data);
void addHead(Node* head, Student data);
void printList(Node head);
int deleteNode(Node* head, char* studentID);
StudentData* searchNode(Node head, char* studentID);
void freeList(Node head);

#endif /* _LINKEDLIST_H_ */


