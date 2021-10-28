//
//  account.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "account.h"

Account createAccount(void) {
  Account account = (Account)malloc(sizeof(AccountData));
  if (account == NULL) {
    printf("\nError: Cannot allocate memory for account\n");
    exit(1);
  }
  return account;
}

void printAccountData(Account account) {
  printf("%s|%s|%d\n", account->username, account->password, account->status);
}