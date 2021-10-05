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
#include "mainFunc.h"
#include "scoreBoard.h"
#include "student.h"

void clearSTDIN(void) {
  while (getchar() != '\n')
    ;
}

int acceptContinue(void) {
  char answer = '\0';
  printf("\nDo you want to continue? (y/n): ");

  scanf("%c", &answer);
  clearSTDIN();
  printf("\n");
  if (answer == 'y' || answer == 'Y') {
    return 1;
  } else {
    return 0;
  }
}

void addScoreBoard(void) {
  printf("\n------ ADD Score Board ------\n");
  ScoreBoard scoreBoard = createScoreBoard();

  printf("Enter subject ID: ");
  scanf("%[^\n]s", scoreBoard->subjectID);
  clearSTDIN();
  printf("Enter subject name: ");
  scanf("%[^\n]s", scoreBoard->subjectName);
  clearSTDIN();
  printf("Enter midterm rate: ");
  scanf("%d", &scoreBoard->midRate);
  clearSTDIN();
  printf("Enter final rate: ");
  scanf("%d", &scoreBoard->finalRate);
  clearSTDIN();
  printf("Enter semester: ");
  scanf("%[^\n]s", scoreBoard->semester);
  clearSTDIN();
  printf("Enter number of student: ");
  scanf("%d", &scoreBoard->numberStudent);
  clearSTDIN();

  initList(&scoreBoard->listStudent);
  for (int i = 0; i < scoreBoard->numberStudent; i++) {
    Student temp = EnterStudentData();
    temp->letterGrade =
        convertScore(temp->midTermScore, temp->finalTermScore,
                     scoreBoard->midRate, scoreBoard->finalRate);

    Student check = searchNode(scoreBoard->listStudent, temp->studentID);
    if (check == NULL) {
      addHead(&scoreBoard->listStudent, temp);
      printf("Add student no.%d: %s success\n", i + 1, temp->studentID);
    } else {
      printf("\nStudent ID: %s already exists!\n", temp->studentID);
      free(temp);
      i--;
    }
  }

  char* fileName = makeFileName(scoreBoard->subjectID, scoreBoard->semester);
  printScoreBoardToFile(scoreBoard, fileName);
  free(fileName);
  freeScoreBoard(scoreBoard);
  printf("----------------------------------\n");
}

void addStudentScore(void) {
  printf("\n------ Add Student Score ------\n");
  char* fileName = findScoreBoardFile();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  printf("\n");
  if (scoreBoard == NULL) {
    printf("Score Board not found!\n\n");
    printf("----------------------------------\n");
    free(fileName);
    return;
  }

  do {
    Student student = EnterStudentData();
    student->letterGrade =
        convertScore(student->midTermScore, student->finalTermScore,
                     scoreBoard->midRate, scoreBoard->finalRate);
    // check if student is already in the list
    Student check = searchNode(scoreBoard->listStudent, student->studentID);
    if (check == NULL) {
      addHead(&scoreBoard->listStudent, student);
      printf("Add student score success\n");
      scoreBoard->numberStudent++;
    } else {
      printf("\nStudent ID: %s already exists!\n", student->studentID);
      free(student);
    }
  } while (acceptContinue());

  printScoreBoardToFile(scoreBoard, fileName);
  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("----------------------------------\n");
}

void deleteStudentScore(void) {
  printf("\n------ Delete Student Score ------\n");
  int check = 0;  // check delete success or not
  char studentID[MAX_LENGTH_STUDENT_ID + 1] = "\0";  // student ID to delete
  char* fileName = findScoreBoardFile();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  printf("\n");
  if (scoreBoard == NULL) {
    printf("Score Board not found!\n\n");
    printf("----------------------------------\n");
    free(fileName);
    return;
  }

  do {
    printf("Enter studentID: ");
    scanf("%s", studentID);
    clearSTDIN();
    check = deleteNode(&scoreBoard->listStudent, studentID);

    if (!check) {
      printf("\nStudentID not found!\n");
    } else {
      printf("\nStudentID %s deleted!\n", studentID);
      scoreBoard->numberStudent--;
    }
  } while (acceptContinue());

  printScoreBoardToFile(scoreBoard, fileName);
  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("\n----------------------------------\n");
}

void searchStudentScore(void) {
  printf("\n------ Search Student Score ------\n");
  char studentID[MAX_LENGTH_STUDENT_ID + 1] = "\0";  // student ID to search
  char* fileName = findScoreBoardFile();
  ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
  printf("\n");
  if (scoreBoard == NULL) {
    printf("Score Board not found!\n\n");
    printf("----------------------------------\n");
    free(fileName);
    return;
  }

  do {
    printf("Enter studentID: ");
    scanf("%s", studentID);
    clearSTDIN();
    Student student = searchNode(scoreBoard->listStudent, studentID);
    if (student == NULL) {
      printf("\nStudentID not found!\n");
    } else {
      printf("\nStudentID %s found!\n", studentID);
      printStudentData(student);
    }
  } while (acceptContinue());

  freeScoreBoard(scoreBoard);
  free(fileName);
  printf("\n----------------------------------\n");
}

void displayScoreBoard(void) {
  printf("\n------ Print Score Board ------\n");
  do {
    char* fileName = findScoreBoardFile();
    ScoreBoard scoreBoard = readScoreBoardFromFile(fileName);
    printf("\n");
    if (scoreBoard == NULL) {
      printf("Score Board not found!\n\n");
      printf("----------------------------------\n");
      free(fileName);
      continue;
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