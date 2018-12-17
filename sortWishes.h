//
// Created by Emil on 17-12-2018.
//

#ifndef FAERDIGT_SORTWISHES_H
#define FAERDIGT_SORTWISHES_H

#include "sortWishes.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "groups.h"

void sortWishes(student studentList[], int numOfStudents, int maxGroups, student **group);
int wishedCmp( const void *a, const void *b);

#endif //FAERDIGT_SORTWISHES_H

