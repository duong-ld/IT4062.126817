//
//  fileFunc.c
//  Ex2
//
//  Created by Lương Dương on 28/9/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileFunc.h"
#include "linkedList.h"

// return subjectID_semester.txt
char* makeFileName(char* subjectID, char* semester) {
  char* result = malloc(strlen(subjectID) + strlen(semester) + strlen(".txt") +
                        strlen("_") + 1);
  if (result == NULL) {
    printf("Error: malloc failed\n");
    exit(1);
  }
  strcpy(result, subjectID);
  strcat(result, "_");
  strcat(result, semester);
  strcat(result, ".txt");
  return result;
}

// ABCD_2017.txt -> ABCD_2017_rp.txt
char* makeReportFile(char* fileName) {
  char* rpFile =
      (char*)malloc(sizeof(char) * (strlen(fileName) + strlen("_rp") + 1));
  if (rpFile == NULL) {
    printf("Error: malloc failed\n");
    exit(1);
  }
  strncpy(rpFile, fileName, strlen(fileName) - 4);
  strcat(rpFile, "_rp.txt");
  return rpFile;
}

char* findScoreBoard(void) {
  char subjectID[20] = "\0";
  char semester[20] = "\0";
  printf("Enter subject ID: ");
  scanf("%[^\n]s", subjectID);
  getchar();  // clear buffer
  printf("Enter semester: ");
  scanf("%[^\n]s", semester);
  getchar();  // clear buffer
  printf("\n");
  char* fileName = makeFileName(subjectID, semester);
  return fileName;
}

StudentData* readStudentFromFile(FILE* fp) {
  StudentData* student = (StudentData*)malloc(sizeof(StudentData));
  // S|20180057|Luong Duc|Duong|10.0|8.0|A|
  fgetc(fp);  // get S
  fgetc(fp);  // get |
  fscanf(fp, "%[^|]s", student->studentID);
  fgetc(fp);  // get |
  fscanf(fp, "%[^|]s", student->firstName);
  fgetc(fp);  // get |
  fscanf(fp, "%[^|]s", student->lastName);
  fgetc(fp);  // get |
  fscanf(fp, "%lf", &student->midTermScore);
  fgetc(fp);  // get |
  fscanf(fp, "%lf", &student->finalTermScore);
  fgetc(fp);  // get |
  fscanf(fp, "%c", &student->letterGrade);
  fgetc(fp);  // get |
  fgetc(fp);  // get \n
  return student;
}

ScoreBoard readScoreBoardFromFile(char* fileName) {
  ScoreBoard scoreBoard;
  strcpy(scoreBoard.subjectID, "NULL");

  FILE* pf = fopen(fileName, "r");
  if (pf == NULL) {
    return scoreBoard;  // return scoreBoard with subjectID = NULL
  }

  fscanf(pf, "%*[^|]s");
  fgetc(pf);  // get |
  fscanf(pf, "%[^\n]s", scoreBoard.subjectID);
  fgetc(pf);  // get \n
  fscanf(pf, "%[^\n]s", scoreBoard.subjectName);
  fgetc(pf);  // get \n
  fgetc(pf);  // get F
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard.midRate);
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard.finalRate);
  fgetc(pf);  // get \n
  fscanf(pf, "%[^\n]s", scoreBoard.semester);
  fgetc(pf);  // get \n
  fscanf(pf, "%*[^|]s");
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard.numberStudent);
  fgetc(pf);  // get \n

  createList(&scoreBoard.head);

  for (int i = 0; i < scoreBoard.numberStudent; i++) {
    StudentData* studentTmp = readStudentFromFile(pf);
    addHead(&scoreBoard.head, studentTmp);
  }

  fclose(pf);
  return scoreBoard;
}

void printStudentToFile(FILE* pf, StudentData student) {
  fprintf(pf, "S|%s|%s|%s|%.1lf|%.1lf|%c|\n", student.studentID,
          student.firstName, student.lastName, student.midTermScore,
          student.finalTermScore, student.letterGrade);
}

void printScoreBoardToFile(ScoreBoard scoreBoard, char* fileName) {
  FILE* pf = fopen(fileName, "w");
  if (pf == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(pf, "SubjectID|%s\n", scoreBoard.subjectID);
  fprintf(pf, "%s\n", scoreBoard.subjectName);
  fprintf(pf, "F|%d|%d\n", scoreBoard.midRate, scoreBoard.finalRate);
  fprintf(pf, "%s\n", scoreBoard.semester);
  fprintf(pf, "StudentCount|%d\n", scoreBoard.numberStudent);

  Node tmp = scoreBoard.head;
  while (tmp != NULL) {
    printStudentToFile(pf, *(tmp->data));
    tmp = tmp->next;
  }
  fclose(pf);
}

void printHistogramToFile(ScoreBoard scoreBoard, int histogram[], FILE* pf) {
  fprintf(pf, "A histogram of the subject %s is:\n", scoreBoard.subjectID);

  for (int i = 0; i < 6; i++) {
    if (i != 4) {
      fprintf(pf, "%c:", i + 'A');
      for (int j = 0; j < histogram[i]; j++) {
        fprintf(pf, "*");
      }
      fprintf(pf, "\n");
    }
  }
}

void exportScoreSummary(ScoreBoard scoreBoard, char* fileName) {
  FILE* pf = fopen(fileName, "w");
  if (pf == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  StudentData maxPoint;
  double max = -100;
  StudentData minPoint;
  double min = 100;
  int histogram[6] = {0};
  double average = 0;
  double sum = 0;
  double grade = 0;  // grade of each student

  Node tmp = scoreBoard.head;
  while (tmp != NULL) {
    grade = calulateScore(tmp->data->midTermScore, tmp->data->finalTermScore,
                          scoreBoard.midRate, scoreBoard.finalRate);
    sum += grade;
    if (max < grade) {
      max = grade;
      maxPoint = *(tmp->data);
    }
    if (min > grade) {
      min = grade;
      minPoint = *(tmp->data);
    }
    histogram[tmp->data->letterGrade - 'A']++;
    tmp = tmp->next;
  }
  average = sum / scoreBoard.numberStudent;
  fprintf(pf, "The student with the highest mark is: %s %s\n",
          maxPoint.firstName, maxPoint.lastName);
  fprintf(pf, "The student with the lowest mark is: %s %s\n",
          minPoint.firstName, minPoint.lastName);
  fprintf(pf, "The average mark is: %.2lf\n\n", average);
  printHistogramToFile(scoreBoard, histogram, pf);
  fclose(pf);
}