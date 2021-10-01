//
//  scoreBoard.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#include "linkedList.h"

typedef struct ScoreBoard {
  char subjectID[100];
  char subjectName[100];
  int midRate;
  int finalRate;
  char semester[100];
  int numberStudent;
  Node head;
} ScoreBoard;

void printScoreBoard(ScoreBoard scoreBoard);
char convertScore(double midtermScore,
                  double finaltermScore,
                  int midrate,
                  int finalrate);
double calulateScore(double midtermScore,
                  double finaltermScore,
                  int midrate,
                  int finalrate);
void freeScoreBoard(ScoreBoard scoreBoard);

#endif


