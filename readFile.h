//
// Created by adrian plesner on 16/12/2018.
//

#ifndef PROGRAM_READFILE_H
#define PROGRAM_READFILE_H

#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

enum sort
{
    error = 0,
    belbin,
    wish
};
typedef enum sort sort;

int getGroupCount(FILE* inFP);
int numberOfStudents(FILE *inFP);
int readFile(student studentList[], int rolesCount[9][2], const int numberOfStudents);
void makeNewInput(FILE *newIn);
sort getMode(FILE* inFP);
role strToRole(const char *inStr);


#endif //PROGRAM_READFILE_H
