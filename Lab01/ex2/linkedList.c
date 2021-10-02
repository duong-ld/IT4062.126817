//
//  linkedList.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedList.h"

void initList(Node* head) {
  *head = NULL;
}

Node createNode(Student data) {
  Node node = (Node)malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;
  return node;
}

void addHead(Node* head, Student data) {
  Node node = createNode(data);
  node->next = *head;
  *head = node;
}

// return 1 if success, 0 if not
int deleteNode(Node* head, char* studentID) {
  Node tmp = *head;
  Node prev = NULL;

  while (tmp != NULL) {
    if (strcmp(tmp->data->studentID, studentID) == 0) {
      if (prev == NULL) {
        *head = tmp->next;
      } else {
        prev->next = tmp->next;
      }
      free(tmp);
      return 1;
    } else {
      prev = tmp;
      tmp = tmp->next;
    }
  }
  return 0;
}

Student searchNode(Node head, char* studentID) {
  Node tmp = head;
  while (tmp != NULL) {
    if (strcmp(tmp->data->studentID, studentID) == 0) {
      return tmp->data;
    } else {
      tmp = tmp->next;
    }
  }
  return NULL;
}

void printList(Node head) {
  Node temp = head;
  printStudentData(temp->data);
  while (temp->next != NULL) {
    temp = temp->next;
    printStudentData(temp->data);
  }
}

void freeList(Node head) {
  if (head != NULL) {
    Node tmp = head;
    head = head->next;
    free(tmp->data);
    free(tmp);
    freeList(head);
  }
}
