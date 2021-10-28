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

Node createNode(Account data) {
  Node node = (Node)malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;
  return node;
}

void addHead(Node* head, Account data) {
  Node node = createNode(data);
  node->next = *head;
  *head = node;
}

Node searchNode(Node head, char* username) {
  Node tmp = head;
  while (tmp != NULL) {
    if (strcmp(tmp->data->username, username) == 0) {
      return tmp;
    } else {
      tmp = tmp->next;
    }
  }
  return NULL;
}

void printList(Node head) {
  Node temp = head;
  printAccountData(temp->data);
  while (temp->next != NULL) {
    temp = temp->next;
    printAccountData(temp->data);
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
