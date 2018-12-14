//
// Created by adrian plesner on 14/12/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "studentsGroups.h"


int getGroupCount(FILE* inFP)
{
    int i, groupAmount = 0, scanRes;

    for(i = 0; i <= UI_LINES_SKIPPED; i++)
    {
        fscanf(inFP, "%*[^\n]");
    }
    scanRes = fscanf(inFP, " %*[^[] [ %d %*[^\n]", &groupAmount);
    if (scanRes == 0)
    {
        printf(" * Fejl i linje %d - gruppeantal. Husk at skrive oensket antal grupper!\n", UI_LINES_SKIPPED);
    }
    else if (groupAmount == 0)
    {
        printf(" * Fejl i linje %d - gruppeantal. Gruppeantal kan ikke vaere 0!\n", UI_LINES_SKIPPED);
    }
    return groupAmount;
}

/*Input:  Textfile with students*/
/*Do:     Find number of students (newlines) from input file*/
/*Output: returns number of students*/
int numberOfStudents(FILE *inFP)
{
    int i = 0, count = 0;
    if(inFP != NULL)
    {
        for (i = getc(inFP); i != EOF; i = getc(inFP))
        {
            if (i == '\n')
            {
                count++;
            }
        }
    }
    rewind(inFP);
    return count - LINES_SKIPPED;
}

/*Input:  Amounts of groups & amounts of students*/
/*Do:     Create array of groups with size determined by number of students and amount of groups */
/*Output: group array*/
student **makeGroup(const int groupAmount, const int studentsCount)
{
    int i, studentsPerGroup = studentsCount / groupAmount;
    if (studentsCount % groupAmount)
    {
        studentsPerGroup++;
    }
    student **groups = calloc(groupAmount, sizeof(student*));
    for (i = 0; i < groupAmount; i++)
    {
        groups[i] = calloc(studentsPerGroup, sizeof(student));
    }
    return groups;
}

/* Input:   */
/* Do:      */
/* Output:  */
void addToGroup(student group[], student *inStudent, const int groupSize)
{
    int i = 0,j;
    while(i < groupSize && !inStudent->isInGroup)
    {
        if(strcmp(group[i].name,"") == 0)
        {
            group[i] = *inStudent;
            inStudent->isInGroup = true;
        }
        else
        {
            i++;
        }
    }
}

/* Input:   */
/* Do:       */
/* Output:  */
int studentsInGroup(const student group[], const int groupSize)
{
    int i = 0;
    while(i < groupSize && strcmp(group[i].name,""))
    {
        i++;
    }
    return i;
};

/* Input:  array of students, int number of students in array */
/* Do:     This function calculates the average ambition level of the group  */
/* Output: double average ambition of group */
double averageAmbitionInGroup(const student group[], const int groupSize )
{
    int i,res = 0;
    for(i = 0; i < groupSize; i++)
    {
        res += group[i].ambitionLevel;
    }
    return (double)res/(double)groupSize;
}

void printGroups(student **groups, const int groupAmount)
{
    int i,j,k, maxStudent = sizeof(groups[0]);
    char roleStrBuffer[3][4];
    for(i = 0; i < groupAmount; i++)
    {

        printf("GRUPPE %d:\n",i+1);
        for(j = 0; j < studentsInGroup(groups[i],maxStudent);j++)
        {
            for(k = 0; k < MAX_ROLES; k++)
            {
                roleToStr(groups[i][j].roles[k],roleStrBuffer[k]);
            }
            printf("%-20s (%d, %-15s, %-15s, %-15s, %-15s, %s, %s, %s)\n",
                   groups[i][j].name,groups[i][j].ambitionLevel,groups[i][j].doWant[0], groups[i][j].doWant[1],
                   groups[i][j].doWant[2],groups[i][j].notWant, roleStrBuffer[0],
                   roleStrBuffer[1],roleStrBuffer[2]);
        }
        printf("\n");

    }
}

void roleToStr(role inRole, char str[])
{
    switch(inRole)
    {
        case 1:
        {
            strcpy(str,"iga");
            break;
        }
        case 2:
        {
            strcpy(str,"org");
            break;
        }
        case 3:
        {
            strcpy(str,"afs");
            break;
        }
        case 4:
        {
            strcpy(str,"ide");
            break;
        }
        case 5:
        {
            strcpy(str,"ana");
            break;
        }
        case 6:
        {
            strcpy(str,"spe");
            break;
        }
        case 7:
        {
            strcpy(str,"kon");
            break;
        }
        case 8:
        {
            strcpy(str,"koo");
            break;
        }
        case 9:
        {
            strcpy(str,"for");
            break;
        }
        default:
        {
            strcpy(str," X ");
            break;
        }
    }
}
