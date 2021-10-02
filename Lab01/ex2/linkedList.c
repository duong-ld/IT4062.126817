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

void createList(Node* head) {
  *head = NULL;
}

Node createNode(StudentData* data) {
  Node node = (Node)malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;
  return node;
}

void addHead(Node* head, StudentData* data) {
  Node node = createNode(data);
  node->next = *head;
  *head = node;
}

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

StudentData* searchNode(Node head, char* studentID) {
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
  printStudentData(*(temp->data));
  while (temp->next != NULL) {
    temp = temp->next;
    printStudentData(*(temp->data));
  }
}

void freeList(Node head) {
  Node tmp = head;
  head = head->next;
  free(tmp->data);
  free(tmp);
  if (head != NULL) {
    freeList(head);
  }
}