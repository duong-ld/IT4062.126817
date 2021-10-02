//
//  main.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "mainFunc.h"

int main(int argc, char const* argv[]) {
MENU:
  printf("Learning Management System\n");
  printf("----------------------------------\n");
  printf("\t1. Add a new score board\n");
  printf("\t2. Add score\n");
  printf("\t3. Remove score\n");
  printf("\t4. Search score\n");
  printf("\t5. Display score board and score report\n");
  printf("Your choice (1-5, other to quit): ");
  char choice;
  scanf("%c", &choice);
  clearSTDIN();
  switch (choice) {
    case '1':
      addScoreBoard();
      break;
    case '2':
      addStudentScore();
      break;
    case '3':
      deleteStudentScore();
      break;
    case '4':
      searchStudentScore();
      break;
    case '5':
      displayScoreBoard();
      break;
    default:
      printf("Bye\n");
      return 0;
  }
  goto MENU;
}
