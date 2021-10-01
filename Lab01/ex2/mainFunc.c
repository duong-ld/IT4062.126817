//
//  mainFunc.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileFunc.h"
#include "linkedList.h"
#include "scoreBoard.h"
#include "studentData.h"

int acceptContinue(void) {
  char answer;
  printf("\nDo you want to continue? (y/n): ");

  scanf("%c", &answer);
  printf("\n");
  while (getchar() != '\n')
    ;
  if (answer == 'y' || answer == 'Y') {
    return 1;
  } else {
    return 0;
  }
}

void addScoreBoard(void) {
  printf("\n------ ADD Score Board ------\n");
  char subjectID[50] = "\0";
  char subjectName[50] = "\0";
  int midrate = 0, finalrate = 0;
  char semester[50] = "\0";
  int numberStudent = 0;

  printf("Enter subject ID: ");
  scanf("%[^\n]s", subjectID);
  getchar(); // clear buffer
  printf("Enter subject name: ");
  scanf("%[^\n]s", subjectName);
  getchar(); // clear buffer
  printf("Enter midterm rate: ");
  scanf("%d", &midrate);
  getchar(); // clear buffer
  printf("Enter final rate: ");
  scanf("%d", &finalrate);
  getchar(); // clear buffer
  printf("Enter semester: ");
  scanf("%[^\n]s", semester);
  getchar(); // clear buffer
  printf("Enter number of student: ");
  scanf("%d", &numberStudent);
  getchar(); // clear buffer

  // create file
  char* fileName = makeFileName(subjectID, semester);
  FILE* pf = fopen(fileName, "w");
  if (pf == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(pf, "SubjectID|%s\n", subjectID);
  fprintf(pf, "Subject|%s\n", subjectName);
  fprintf(pf, "F|%d|%d\n", midrate, finalrate);
  fprintf(pf, "Semester|%s\n", semester);
  fprintf(pf, "StudentCount|%d\n", numberStudent);

  StudentData* temp = NULL;
  for (int i = 0; i < numberStudent; i++) {
    temp = EnterStudentData();
    temp->letterGrade = convertScore(temp->midTermScore, temp->finalTermScore,
                                     midrate, finalrate);
    printStudentToFile(pf, *temp);
    free(temp);
  }
  free(fileName);
  fclose(pf);
  printf("-------------------------------------\n");
}

void addStudentScore(void) {
  printf("\n------ Add Student Score ------\n");
  char* fileName = findScoreBoard();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  if (strcmp(scoreBoard.subjectID, "NULL") == 0) {
    printf("Score Board not found!\n");
    printf("-------------------------------------\n");
    return;
  }

  do {
    StudentData* student = EnterStudentData();
    student->letterGrade =
        convertScore(student->midTermScore, student->finalTermScore,
                     scoreBoard.midRate, scoreBoard.finalRate);
    // check if student is already in the list
    StudentData* check = searchNode(scoreBoard.head, student->studentID);
    if (check == NULL) {
      addHead(&scoreBoard.head, student);
      printf("Add student score success\n");
      scoreBoard.numberStudent++;
    } else {
      printf("\nStudent ID already exists!\n");
      free(student);
    }
  } while (acceptContinue());

  printScoreBoardToFile(scoreBoard, fileName);
  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("\n----------------------------------\n");
}

void deleteStudentScore(void) {
  printf("\n------ Delete Student Score ------\n");
  int check = 0;              // check delete success or not
  char studentID[10] = "\0";  // student ID to delete
  char* fileName = findScoreBoard();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  if (strcmp(scoreBoard.subjectID, "NULL") == 0) {
    printf("Score Board not found!\n");
    printf("-------------------------------------\n");
    return;
  }

  do {
    printf("Enter studentID: ");
    scanf("%s", studentID);
    getchar();
    check = deleteNode(&scoreBoard.head, studentID);

    if (!check) {
      printf("\nStudentID not found!\n");
    } else {
      printf("\nStudentID %s deleted!\n", studentID);
      scoreBoard.numberStudent--;
    }
  } while (acceptContinue());

  printScoreBoardToFile(scoreBoard, fileName);
  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("\n----------------------------------\n");
}

void searchStudentScore(void) {
  printf("\n------ Search Student Score ------\n");
  char studentID[10] = "\0";  // student ID to search
  char* fileName = findScoreBoard();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  if (strcmp(scoreBoard.subjectID, "NULL") == 0) {
    printf("Score Board not found!\n");
    printf("-------------------------------------\n");
    return;
  }

  do {
    printf("Enter studentID: ");
    scanf("%s", studentID);
    getchar();
    StudentData* student = searchNode(scoreBoard.head, studentID);
    if (student == NULL) {
      printf("\nStudentID not found!\n");
    } else {
      printf("\nStudentID %s found!\n", studentID);
      printStudentData(*student);
    }
  } while (acceptContinue());
  
  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("\n----------------------------------\n");
}

void displayScoreBoard(void) {
  printf("\n------ Print Score Board ------\n");
  do {
    char* fileName = findScoreBoard();
    ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
    if (strcmp(scoreBoard.subjectID, "NULL") == 0) {
      printf("Score Board not found!\n");
      printf("-------------------------------------\n");
      return;
    }
    printScoreBoard(scoreBoard);
    char* reportFile = makeReportFile(fileName);
    exportScoreSummary(scoreBoard, reportFile);
    free(reportFile);
    freeScoreBoard(scoreBoard);
    free(fileName);
  } while (acceptContinue());
  printf("\n----------------------------------\n");
}