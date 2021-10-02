//
//  scoreBoard.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#include "linkedList.h"
#include "constain.h"

typedef struct ScoreBoard {
  char subjectID[MAX_LENGTH_STUDENT_ID + 1];
  char subjectName[MAX_LENGTH_STUDENT_NAME + 1];
  int midRate;
  int finalRate;
  char semester[MAX_LENGTH_SEMESTER + 1];
  int numberStudent;
  Node listStudent;
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

#endif /* _SCOREBOARD_H_ */


