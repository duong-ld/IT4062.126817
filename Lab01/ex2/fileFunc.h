//
//  fileFunc.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _FILEFUNC_H_
#define _FILEFUNC_H_

#include "scoreBoard.h"

char* makeFileName(char* subjectID, char* semester);
char* makeReportFile(char* fileName);
char* findScoreBoardFile(void);
Student readStudentFromFile(FILE* fp);
ScoreBoard readScoreBoardFromFile(char* fileName);
void printStudentToFile(FILE* pf, Student student);
void printScoreBoardToFile(ScoreBoard scoreBoard, char* fileName);
void exportScoreSummary(ScoreBoard scoreBoard, char* fileName);

#endif /* _FILEFUNC_H_ */
