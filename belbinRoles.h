//
// Created by adrian plesner on 14/12/2018.
//

#include "studentsGroups.h"
#ifndef PROGRAM_BELBINROLES_H
#define PROGRAM_BELBINROLES_H

void sortBelbin(student studentList[], int rolesCount[9][2], const int numberOfStudents, const int groupAmount, student **groups);
bool studentHasRole(const role inRole, const student *inStudent);
bool groupMissingRole(const student group[], const role inRole, const int groupSize);
int findBestGroup(const student *inStudent, student **groups, const int groupAmount, const int groupSize);

int rolesCmp(const void *a, const void *b);
int ambitionCmp(const void *a, const void *b);

#endif //PROGRAM_BELBINROLES_H
