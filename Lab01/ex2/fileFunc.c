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
#include "mainFunc.h"

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
  strncpy(rpFile, fileName, strlen(fileName) - strlen(".txt"));
  strcat(rpFile, "_rp.txt");
  return rpFile;
}

char* findScoreBoardFile(void) {
  char subjectID[MAX_LENGTH_SUBJECT_ID + 1] = "\0";
  char semester[MAX_LENGTH_SEMESTER + 1] = "\0";
  printf("Enter subject ID: ");
  scanf("%[^\n]s", subjectID);
  clearSTDIN();
  printf("Enter semester: ");
  scanf("%[^\n]s", semester);
  clearSTDIN();
  char* fileName = makeFileName(subjectID, semester);
  return fileName;
}

Student readStudentFromFile(FILE* fp) {
  Student student = createStudent();
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

void getStringFieldFromFile(FILE* pf, char* field) {
  // NameField|ValueField
  fscanf(pf, "%*[^|]s");
  fgetc(pf);  // get |
  fscanf(pf, "%[^\n]s", field);
  fgetc(pf);  // get \n
}

ScoreBoard readScoreBoardFromFile(char* fileName) {
  ScoreBoard scoreBoard = createScoreBoard();

  FILE* pf = fopen(fileName, "r");
  if (pf == NULL) {
    freeScoreBoard(scoreBoard);
    return NULL;  // return scoreBoard with subjectID = NULL
  }

  getStringFieldFromFile(pf, scoreBoard->subjectID);
  getStringFieldFromFile(pf, scoreBoard->subjectName);

  // get score rate
  // F|midRate|finalRate
  fgetc(pf);  // get F
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard->midRate);
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard->finalRate);
  fgetc(pf);  // get \n
  // end get score rate

  getStringFieldFromFile(pf, scoreBoard->semester);

  // get number of students
  fscanf(pf, "%*[^|]s");
  fgetc(pf);  // get |
  fscanf(pf, "%d", &scoreBoard->numberStudent);
  fgetc(pf);  // get \n
  // end get number of students

  initList(&scoreBoard->listStudent);
  for (int i = 0; i < scoreBoard->numberStudent; i++) {
    Student studentTmp = readStudentFromFile(pf);
    addHead(&scoreBoard->listStudent, studentTmp);
  }

  fclose(pf);
  return scoreBoard;
}

void printStudentToFile(FILE* pf, Student student) {
  fprintf(pf, "S|%s|%s|%s|%.1lf|%.1lf|%c|\n", student->studentID,
          student->firstName, student->lastName, student->midTermScore,
          student->finalTermScore, student->letterGrade);
}

void printScoreBoardToFile(ScoreBoard scoreBoard, char* fileName) {
  FILE* pf = fopen(fileName, "w");
  if (pf == NULL) {
    freeScoreBoard(scoreBoard);
    printf("Error opening file!\n");
    exit(1);
  }

  if (scoreBoard == NULL) {
    printf("Error: scoreBoard is NULL\n");
    return;
  }

  fprintf(pf, "SubjectID|%s\n", scoreBoard->subjectID);
  fprintf(pf, "Subject|%s\n", scoreBoard->subjectName);
  fprintf(pf, "F|%d|%d\n", scoreBoard->midRate, scoreBoard->finalRate);
  fprintf(pf, "Semester|%s\n", scoreBoard->semester);
  fprintf(pf, "StudentCount|%d\n", scoreBoard->numberStudent);

  Node tmp = scoreBoard->listStudent;
  while (tmp != NULL) {
    printStudentToFile(pf, tmp->data);
    tmp = tmp->next;
  }
  fclose(pf);
}

void printHistogramToFile(ScoreBoard scoreBoard, int histogram[], FILE* pf) {
  fprintf(pf, "A histogram of the subject %s is:\n", scoreBoard->subjectID);

  for (int i = 0; i < 6; i++) {
    // dont have E point in histogram
    if (i != 'E' - 'A') {
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
    freeScoreBoard(scoreBoard);
    printf("Error opening file!\n");
    exit(1);
  }
  if (scoreBoard == NULL) {
    printf("Error: scoreBoard is NULL\n");
    return;
  }

  StudentData maxPoint;
  double max = -100;
  StudentData minPoint;
  double min = 100;
  // A -> F
  int histogram[6] = {0};
  double average = 0;
  double sum = 0;
  double grade = 0;  // grade of each student

  Node tmp = scoreBoard->listStudent;
  while (tmp != NULL) {
    grade = calulateScore(tmp->data->midTermScore, tmp->data->finalTermScore,
                          scoreBoard->midRate, scoreBoard->finalRate);
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
  average = sum / scoreBoard->numberStudent;
  fprintf(pf, "The student with the highest mark is: %s %s\n",
          maxPoint.firstName, maxPoint.lastName);
  fprintf(pf, "The student with the lowest mark is: %s %s\n",
          minPoint.firstName, minPoint.lastName);
  fprintf(pf, "The average mark is: %.2lf\n\n", average);
  printHistogramToFile(scoreBoard, histogram, pf);
  fclose(pf);
}