//
//  studentData.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "mainFunc.h"
#include "student.h"

Student createStudent(void) {
  Student student = (Student)malloc(sizeof(StudentData));
  if (student == NULL) {
    printf("\nError: Cannot allocate memory for student\n");
    exit(1);
  }
  return student;
}

Student EnterStudentData(void) {
  Student student = (Student)malloc(sizeof(StudentData));

  printf("\n------ Enter Student Information ------\n");
  printf("Enter student ID: ");
  scanf("%[^\n]s", student->studentID);
  clearSTDIN();
  printf("Enter first name: ");
  scanf("%[^\n]s", student->firstName);
  clearSTDIN();
  printf("Enter last name: ");
  scanf("%[^\n]s", student->lastName);
  clearSTDIN();
ENTER_SCORE:
  printf("Enter midterm score: ");
  scanf("%lf", &student->midTermScore);
  clearSTDIN();
  printf("Enter finalterm score: ");
  scanf("%lf", &student->finalTermScore);
  clearSTDIN();
  if (student->midTermScore < 0 || student->midTermScore > 10 ||
      student->finalTermScore < 0 || student->finalTermScore > 10) {
    printf("\nError: Invalid score! Re type score\n");
    goto ENTER_SCORE;
  }
  printf("\n---------------------------------------\n\n");
  return student;
}

void printStudentData(Student student) {
  printf("S|%s|%s\t|%s\t| %.1lf\t| %.1lf\t| %c\t|\n", student->studentID,
         student->firstName, student->lastName, student->midTermScore,
         student->finalTermScore, student->letterGrade);
}