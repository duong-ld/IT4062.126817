//
//  scoreBoard.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "scoreBoard.h"
#include "studentData.h"

void printScoreBoard(ScoreBoard scoreBoard) {
  printf("SubjectID|%s\n", scoreBoard.subjectID);
  printf("%s\n", scoreBoard.subjectName);
  printf("F|%d|%d\n", scoreBoard.midRate, scoreBoard.finalRate);
  printf("%s\n", scoreBoard.semester);
  printf("StudentCount|%d\n", scoreBoard.numberStudent);

  Node tmp = scoreBoard.head;
  while (tmp != NULL) {
    printStudentData(*(tmp->data));
    tmp = tmp->next;
  }
}

double calulateScore(double midtermScore,
                  double finaltermScore,
                  int midrate,
                  int finalrate) {
  return (midrate * midtermScore + finalrate * finaltermScore) / 100;
}

char convertScore(double midtermScore,
                  double finaltermScore,
                  int midrate,
                  int finalrate) {
  double grade =
      calulateScore(midtermScore, finaltermScore, midrate, finalrate);

  if (grade < 0) {
    printf("Invalid score >> F\n");
    return 'F';
  }

  if (grade < 4.0)
    return 'F';
  else if (grade < 5.5)
    return 'D';
  else if (grade < 7.0)
    return 'C';
  else if (grade < 8.5)
    return 'B';
  else
    return 'A';
}

void freeScoreBoard(ScoreBoard scoreBoard) {
  freeList(scoreBoard.head);
}