//
//  studentData.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "studentData.h"

StudentData* EnterStudentData(void) {
  StudentData* student = (StudentData*)malloc(sizeof(StudentData));

  printf("\n------ Enter Student Information ------\n");
  printf("Enter student ID: ");
  scanf("%[^\n]s", student->studentID);
  getchar();
  printf("Enter first name: ");
  scanf("%[^\n]s", student->firstName);
  getchar();
  printf("Enter last name: ");
  scanf("%[^\n]s", student->lastName);
  getchar();
  printf("Enter midterm score: ");
  scanf("%lf", &student->midTermScore); getchar();
  printf("Enter finalterm score: ");
  scanf("%lf", &student->finalTermScore); getchar();
  printf("\n------------------------------------------\n\n");
  return student;
}

void printStudentData(StudentData student) {
  printf("S|%s|%s\t|%s\t| %.1lf\t| %.1lf\t| %c\t|\n", student.studentID, student.firstName,
         student.lastName, student.midTermScore, student.finalTermScore,
         student.letterGrade);
}