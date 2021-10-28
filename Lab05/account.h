//
//  account.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

typedef struct AccountData {
  char username[100];
  char password[100];
  int status;
  int countLogin;
} AccountData;

typedef AccountData* Account;

Account createAccount(void);
void printAccountData(Account account);

#endif /* _ACCOUNT_H_ */


