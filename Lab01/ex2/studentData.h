//
//  studentData.h
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#ifndef _STUDENTDATA_H_
#define _STUDENTDATA_H_

typedef struct StudentData StudentData;
struct StudentData {
  char studentID[10];
  char firstName[50];
  char lastName[50];
  double midTermScore;
  double finalTermScore;
  char letterGrade;
};

StudentData* EnterStudentData(void);
void printStudentData(StudentData student);

#endif


