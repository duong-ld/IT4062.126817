//
//  studentData.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _STUDENTDATA_H_
#define _STUDENTDATA_H_

#include "constain.h"

typedef struct StudentData StudentData;
struct StudentData {
  char studentID[MAX_LENGTH_STUDENT_ID + 1];
  char firstName[MAX_LENGTH_STUDENT_NAME + 1];
  char lastName[MAX_LENGTH_STUDENT_NAME + 1];
  double midTermScore;
  double finalTermScore;
  char letterGrade;
};

StudentData* EnterStudentData(void);
void printStudentData(StudentData student);

#endif /* _STUDENTDATA_H_ */


