//
// Created by adrian plesner on 14/12/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "belbinRoles.h"



/* Input:  Array of students, array of amount of roles, number of students, number of groups, array of groups */
/* Do:     Sort students into groups by their Belbin groups roles  */
/* Output: Array of groups */
void sortBelbin(student studentList[], int rolesCount[9][2], const int numberOfStudents, const int groupAmount, student **groups)
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
            if (!studentList[i].isInGroup) {
                j = findBestGroup(&studentList[i], groups, groupAmount, studentPerGroup + 1);
                addToGroup(groups[j], &studentList[i], studentPerGroup + 1);
            }


        }
    }

}

/* Input:  A group role, a student  */
/* Do:     Determines if the student has the role */
/* Output: Bool if the student has the role or not */
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

/* Input:  A groups of students, a group role, amount of students in the group */
/* Do:     Determines if some of the students in the group has the group role  */
/* Output: Bool if the group role is in the group */
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

/* Input:  A student, array of groups, amount of groups, amount of student in each group */
/* Do:     Determines which group the student fits best into, based on group roles, amount of students already in the group and ambition level */
/* Output: The index number of the group in array groups, in which the student fits best */
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

/* Input:  Two numbers representing the amount af two groups roles */
/* Do:     This function compares the amount of Belbin roles    */
/* Output: Returns which has the lowest amount */
int rolesCmp(const void *a, const void *b)
{
    int *numa = (int*)a;
    int *numb = (int*)b;
    return (numa[1] - numb[1]);
}

/* Input:  Two students */
/* Do:     This function compares students by ambition level */
/* Output: Returns which student the highest ambition level */
int ambitionCmp(const void *a, const void *b)
{
    student *pa = (student*)a;
    student *pb = (student*)b;
    return (pb->ambitionLevel - pa->ambitionLevel);
}
