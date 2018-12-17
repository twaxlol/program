//
// Created by Emil on 17-12-2018.
//

#include "sortWishes.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "groups.h"

void sortWishes(student studentList[], int numOfStudents, int maxGroups, student **group)
{

    int groupsCount = 0;
    int maxMembers = numOfStudents / maxGroups;
    if(numOfStudents % maxMembers != 0)
    {
        maxMembers++;
    }
    int loopStart = 0;
    int i, j, k, l, iteration;
    student *tempGroups[maxGroups];

    int groupSizes[maxGroups];
    int avgAmbition[maxGroups];

    for(i = 0; i < numOfStudents; i++)
    {
        studentList[i].wishedAmount = 0;
        studentList[i].isInGroup = false;
    }
    for(i = 0; i < maxGroups; i++)
    {
        groupSizes[i] = 0;
    }

    /* Count the times the different students have been wished */
    for(i = 0; i < numOfStudents; i++)
    {
        for(j = 0; j < numOfStudents; j++)
        {
            for(k = 0; k < 3; k++)
            {
                if(strcmp(studentList[i].name, studentList[j].doWant[k]) == 0)
                {
                    strcpy(studentList[i].wishedBy[studentList[i].wishedAmount], studentList[j].name);
                    studentList[i].wishedAmount++;
                }
            }
        }
    }

    qsort(studentList, numOfStudents , sizeof(student), wishedCmp);


    for(i = 0; i < numOfStudents; i++)
    {
        if(studentList[i].wishedAmount == 0)
        {
            loopStart++;
        }
    }

    /* assign a person to each group. starting from the least wished (besides those that hasnt been wished for */
    for(i = 0; i < maxGroups; i++)
    {
        group[i][0] = studentList[i + loopStart];
        studentList[i + loopStart].isInGroup = true;
        groupSizes[i]++;
        groupsCount++;
    }
    printf("\n");

/* Check if they have a mutural wish, meaning if they are wanted by a person that they also wish to be in a group with */
    for(i = 0; i < groupsCount; i++)
    {
        for(j = 0; j < 3; j++)
        {
            for(k = 0; k < numOfStudents; k++)
            {
                if(!studentList[k].isInGroup)
                {
                    if(strcmp(studentList[k].name, group[i][0].doWant[j]) == 0 && strcmp(group[i][0].name, studentList[k].doWant[j]) == 0 && groupSizes[i] < 2)
                    {
                        group[i][1] = studentList[k];
                        studentList[k].isInGroup = true;
                        groupSizes[i]++;
                    }
                }
            }
        }
    }

    /* if a person cant be matched with a mutural wish, they get a wish instead */

    for(i = 0; i < groupsCount; i++)
    {
        if(groupSizes[i] < 2)
        {
            for(j = 0; j < 3; j++)
            {
                for(k = 0; k < numOfStudents; k++)
                {
                    if(!studentList[k].isInGroup && strcmp(studentList[k].name, group[i][0].doWant[j]) == 0 && groupSizes[i] < 2)
                    {
                        group[i][1] = studentList[k];
                        studentList[k].isInGroup = true;
                        groupSizes[i]++;
                    }
                }
            }
        }
    }

/* If group is still less than 2, see who theyre wished by instead*/
    for(i = 0; i < groupsCount; i++)
    {
        if(groupSizes[i] < 2)
        {
            for(j = 0; j < group[i][0].wishedAmount; j++)
            {
                for(k = 0; k < numOfStudents; k++)
                {
                    if(!studentList[k].isInGroup && strcmp(group[i][0].wishedBy[j], studentList[k].name) == 0 && groupSizes[i] < 2)
                    {
                        group[i][1] = studentList[k];
                        studentList[k].isInGroup = true;
                        groupSizes[i]++;
                    }
                }
            }
        }
    }

    /* If there still is a person without a first partner, add someone that werent wished */
    for(i = 0; i < groupsCount; i++)
    {
        if(groupSizes[i] < 2)
        {
            for(j = 0; j < numOfStudents; j++)
            {
                if(!studentList[j].isInGroup && studentList[j].wishedAmount == 0 && groupSizes[i] < 2)
                {
                    group[i][1] = studentList[j];
                    studentList[j].isInGroup = true;
                }
            }
        }
    }

    int predictedGroup = -1, highestPoints = 0, currentPoints = 0;

    for(iteration = 3; iteration < maxMembers; iteration++)
    {
        for (i = 0; i < numOfStudents; i++)
        {
            if (!studentList[i].isInGroup && studentList[i].wishedAmount > 0)
            {
                for (j = 0; j < groupsCount; j++)
                {
                    if (groupSizes[j] < iteration)
                    {
                        for (k = 0; k < groupSizes[j]; k++)
                        {
                            for (l = 0; l < 3; l++) {
                                if (strcmp(studentList[i].name, group[j][k].doWant[l]) == 0 || strcmp(studentList[i].doWant[l], group[j][k].name) == 0)
                                {
                                    currentPoints++;
                                }
                            }
                        }

                        if (groupSizes[j] < iteration && currentPoints > highestPoints)
                        {
                            highestPoints = currentPoints;
                            predictedGroup = j;
                        }

                    }
                }

                if (predictedGroup != -1) {
                    group[predictedGroup][iteration - 1] = studentList[i];
                    studentList[i].isInGroup = true;
                    groupSizes[predictedGroup]++;
                }
                predictedGroup = -1;
                highestPoints = 0;
            }
        }
    }

    /*Take care of the people without any person that wished for them by first checking who they wished for, again with a point assignment system*/
    currentPoints = 0;
    predictedGroup = -1;
    highestPoints = 0;

    for(i = 0; i < numOfStudents; i++)
    {
        if(!studentList[i].isInGroup)
        {
            for(j = 0; j < groupsCount; j++)
            {
                if(groupSizes[j] == maxMembers - 2)
                {
                    for(k = 0; k < maxMembers - 1; k++)
                    {
                        for(l = 0; l < 3; l++)
                        {
                            if(strcmp(studentList[i].doWant[l], group[j][k].name) == 0)
                            {
                                currentPoints++;
                            }
                        }
                    }
                }
                if(currentPoints != 0 && currentPoints > highestPoints)
                {
                    highestPoints = currentPoints;
                    predictedGroup = j;
                }
                currentPoints = 0;
            }

            if(highestPoints != 0)
            {
                group[predictedGroup][maxMembers - 2] = studentList[i];
                studentList[i].isInGroup = true;
                groupSizes[predictedGroup]++;

            }
            highestPoints = 0;
            predictedGroup = -1;
        }
    }

    /* Calculate average Ambitionlevel of every group */
    int total = 0;
    int avg = 0;
    for(int i = 0; i < groupsCount; i++)
    {
        for(int j = 0; j < groupSizes[i]; j++)
        {
            total += group[i][j].ambitionLevel;
        }
        avg = total / groupSizes[i];
        avgAmbition[i] = avg;
        avg = 0;
        total = 0;
    }

    /*Take care of the remainder, meaning the groups with one extra person in it by looking at ambition level*/
    int lowestDiff = 99999;
    int currDiff = 0;
    int lowestGroup = -1;
    for(int i = 0; i < numOfStudents; i++)
    {
        if(!studentList[i].isInGroup)
        {
            for(int j = 0; j < groupsCount; j++)
            {
                if(groupSizes[j] == maxMembers - 1 && !studentList[i].isInGroup)
                {
                    if(studentList[i].ambitionLevel > avgAmbition[j])
                    {
                        currDiff = studentList[i].ambitionLevel - avgAmbition[j];
                    }
                    else if(studentList[i].ambitionLevel < avgAmbition[j])
                    {
                        currDiff = avgAmbition[j] - studentList[i].ambitionLevel ;
                    }
                }

                if(currDiff < lowestDiff)
                {
                    lowestDiff = currDiff;
                    lowestGroup = j;
                }

            }

            studentList[i].isInGroup = true;
            group[lowestGroup][groupSizes[lowestGroup]] = studentList[i];
            groupSizes[lowestGroup]++;
        }

    }

}

int wishedCmp( const void *a, const void *b)
{
    student *pa = (student*)a;
    student *pb = (student*)b;
    return (pb->wishedAmount - pa->wishedAmount);
}
