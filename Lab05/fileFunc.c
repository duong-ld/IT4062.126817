//
//  fileFunc.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileFunc.h"

Account readAccountFromFile(FILE* fp) {
  Account account = createAccount();
  // hust hust123 1
  fscanf(fp, "%s", account->username);
  if (account->username[0] == '\0' || account->username[0] == '\n') {
    free(account);
    return NULL;
  }
  fgetc(fp);
  fscanf(fp, "%s", account->password);
  if (account->password[0] == '\0' || account->password[0] == ' ') {
    free(account);
    return NULL;
  }
  fgetc(fp);
  fscanf(fp, "%d", &account->status);
  fgetc(fp);
  account->countLogin = 0;
  return account;
}

void readListAccountFromFile(char* fileName, Node* head) {
  FILE* pf = fopen(fileName, "r");
  if (pf == NULL) {
    printf("Error opening file!\n");
    return;
  }

  while (!feof(pf)) {
    Account account = readAccountFromFile(pf);
    if (account != NULL) {
      addHead(head, account);
    } else {
      break;
    }
  }
  fclose(pf);
}

void printAccountToFile(FILE* pf, Account account) {
  if (pf == NULL) {
    printf("Error: file is NULL\n");
    return;
  }

  if (account == NULL) {
    printf("Error: Account is NULL\n");
    return;
  }

  fprintf(pf, "%s %s %d\n", account->username, account->password,
          account->status);
}

void  printListAccountToFile(char* fileName, Node head) {
  FILE* pf = fopen(fileName, "w");
  if (pf == NULL) {
    printf("Error opening file!\n");
    return;
  }


  Node p = head;
  while (p != NULL) {
    printAccountToFile(pf, p->data);
    p = p->next;
  }
  fclose(pf);
}