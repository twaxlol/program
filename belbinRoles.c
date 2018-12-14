//
// Created by adrian plesner on 14/12/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "belbinRoles.h"



/* Input:   */
/* Do:       */
/* Output:  */
void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents, int groupAmount, student **groups)
{
    int i,j,k, studentPerGroup = numberOfStudents/groupAmount;
    qsort(rolesCount,9 ,2*sizeof(int),rolesCmp);
    qsort(studentList, numberOfStudents ,sizeof(student), ambitionCmp);


    for (i = 0; i < 9; i++)
    {

        int studentIndex = 0;
        j = 0;
        while(j < rolesCount[i][1] && j < groupAmount && studentIndex < numberOfStudents)
        {
            if(!studentList[studentIndex].isInGroup && studentHasRole(rolesCount[i][0],&studentList[studentIndex]) && groupMissingRole(groups[j],rolesCount[i][0],studentPerGroup))
            {
                addToGroup(groups[j],&studentList[studentIndex],studentPerGroup);
                j++;
            }
            studentIndex++;
        }
    }
    for(i = 0; i < numberOfStudents; i++)
    {
        if(!studentList[i].isInGroup)
        {
            j = 0;
            while(j < groupAmount && !studentList[i].isInGroup)
            {
                if(studentsInGroup(groups[j],studentPerGroup) < studentPerGroup)
                {
                    k = 0;
                    while (k < MAX_ROLES && !studentList[i].isInGroup)
                    {
                        if(groupMissingRole(groups[j],studentList[i].roles[k],studentPerGroup))
                        {
                            addToGroup(groups[j],&studentList[i],numberOfStudents);
                        }
                        else
                        {
                            k++;
                        }
                    }
                }
                j++;

            }
            j = findBestGroup(&studentList[i],groups,groupAmount,studentPerGroup+1);
            addToGroup(groups[j],&studentList[i],studentPerGroup+1);
            i++;
        }
    }
}

/* Input:   */
/* Do:       */
/* Output:  */
bool studentHasRole(const role inRole, const student *inStudent)
{
    int i, res = 0;
    for(i = 0; i < MAX_ROLES; i++)
    {
        if(inStudent->roles[i] == inRole)
        {
            res++;
        }
    }
    if(res)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Input:   */
/* Do:       */
/* Output:  */
bool groupMissingRole(const student group[], const role inRole, const int groupSize)
{
    int i,j,res = 0;
    for(i = 0; i < groupSize; i++)
    {
        for(j = 0; j < MAX_ROLES; j++)
        {
            if(group[i].roles[j] == inRole)
            {
                res++;
            }
        }

    }
    if(!res)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Input:   */
/* Do:       */
/* Output:  */
int findBestGroup(const student *inStudent, student **groups, const int groupAmount, const int groupSize)
{
    int i,j,res = 0, bufferA,bufferB;
    double dBufferA, dBufferB;
    for(i = 1; i < groupAmount; i++)
    {
        bufferA = studentsInGroup(groups[i],groupSize) - studentsInGroup(groups[res],groupSize);
        if(bufferA < 0)
        {
            res = i;
        }
        else if(bufferA == 0)
        {
            bufferB = 0;
            for(j = 0; j < MAX_ROLES; j++)
            {
                bufferA += groupMissingRole(groups[res],inStudent->roles[j],groupSize);
                bufferB += groupMissingRole(groups[i],inStudent->roles[j],groupSize);

            }
            bufferA -= bufferB;
            if(bufferA < 0)
            {
                res = i;
            }
            else if(bufferA == 0)
            {
                dBufferA = fabs(inStudent->ambitionLevel - averageAmbitionInGroup(groups[res],studentsInGroup(groups[res],groupAmount)));
                dBufferB = fabs(inStudent->ambitionLevel - averageAmbitionInGroup(groups[i],studentsInGroup(groups[i],groupAmount)));
                dBufferA -= dBufferB;
                if(dBufferA > 0)
                {
                    res = i;
                }
            }
        }
    }
    return res;
}

/* Input:  Student struct */
/* Do:     This function compares the amount of Belbin roles  */
/* Output: returns the lowest amount of Belbin roles */
int rolesCmp(const void *a, const void *b)
{
    int *numa = (int*)a;
    int *numb = (int*)b;
    return (numa[1] - numb[1]);
}

/* Input:  Student struct */
/* Do:     This function compares students ambition level */
/* Output: returns the highest ambition level */
int ambitionCmp(const void *a, const void *b)
{
    student *pa = (student*)a;
    student *pb = (student*)b;
    return (pb->ambitionLevel - pa->ambitionLevel);
}
