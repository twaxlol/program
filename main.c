/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "strlwr.h"

#define MAX_ROLES 3
#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

/* Define enum and structs */
enum role
{
    iga = 0, /* Igangsætter */
    org = 1, /* Organisator */
    afs = 2, /* Afslutter */
    ide = 3, /* Ideskaber */
    ana = 4, /* Analysator */
    spe = 5, /* Specialist */
    kon = 6, /* Kontaktskaber */
    koo = 7, /* Koordinator */
    frm = 8  /* Formidler */
};
typedef enum role role;

enum sort
{
    error = 0,
    belbin,
    wish
};
typedef enum sort sort;

struct student
{
    char name[30];
    int ambitionLevel; /*from 1 to 5*/
    role roles[MAX_ROLES];
    char doWant[3][30];
    char notWant[30];
    bool isInGroup;
};

typedef struct student student;

/* Declare fuction prototypes */
int getGroupCount(FILE* inFP);
sort getMode(FILE* inFP);
int numberOfStudents(FILE *file);
student **makeGroup(int groupAmount, int studentsCount);
int readFile(student studentList[], int rolesCount[9][2], int lines);
void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents);
void sortWishes(student studentList[], int numberOfStudents, int groupAmount, student group);
int rolesCmp(const void *a, const void *b);
int ambitionCmp(const void *a, const void *b);



int main(void)
{
    int rolesCount[9][2] =
            {
                    {iga, 0},
                    {org, 0},
                    {afs, 0},
                    {ide, 0},
                    {ana, 0},
                    {spe, 0},
                    {kon, 0},
                    {koo, 0},
                    {frm, 0}
            };

    FILE *inFP = fopen("input.txt","r");

    sort sortMode = error;
    int groupAmount = getGroupCount(inFP);
    sortMode = getMode(inFP);
    if (groupAmount == 0 || sortMode == error)
    {
        return 1;
    }
    rewind(inFP);

    int studentsCount = numberOfStudents(inFP);
    student studentList[studentsCount];
    student **groups = makeGroup(groupAmount, studentsCount);

    int state = readFile(studentList, rolesCount, studentsCount);
    if (state != 0)
    {
        return 1;
    }

    if( sortMode == belbin)
    {
        /*makeBelbinGroups();*/
        sortBelbin(studentList, rolesCount, studentsCount);
    }
    else if( sortMode == wish)
    {
        sortWishes(studentList, studentsCount, groupAmount, groups);
    }
    else{
        printf("FEJL prøv igen :)\n");
    }
    return 0;
}

/* Check amount of groups from input */
int getGroupCount(FILE* inFP)
{
    int i, groupAmount = 0, scanRes;
    for(i = 0; i <= UI_LINES_SKIPPED; i++)
    {
        fscanf(inFP, "%*[^\n]");
    }
    scanRes = fscanf(inFP, "%*[^[] [ %d %*[^\n]", &groupAmount);
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

/* Check mode (Belbin or Wish) from input */
sort getMode(FILE* inFP)
{
    int i;
    char optionFirst, optionSecond;
    for(i = 0; i < 2; i++)
    {
        fscanf(inFP, "%*[^\n]");
    }
    fscanf(inFP, "%*[^[][ %c%*[^\n]", &optionFirst);
    fscanf(inFP, "%*[^[][ %c%*[^\n]", &optionSecond);

    if (tolower(optionFirst) == 'x' && tolower(optionSecond) != 'x')
    {
        return belbin;
    }
    else if (tolower(optionFirst) != 'x' && tolower(optionSecond) == 'x')
    {
        return wish;
    }
    printf("[%c] [%c]\n", optionFirst, optionSecond);
    printf(" * Fejl i linje 28/29 - prioritetsmode. Saet et enkelt kryds (x)!\n");
    return error;
}

/* Find number of students from input */
int numberOfStudents(FILE *inFP){
    int i = 0, count = 0;
    if(inFP != NULL)
    {
        for (i = getc(inFP); i != EOF; i = getc(inFP)) /*Tæller linjer indtil vi rammer EOF*/
        {
            if (i == '\n'){
                count++;
            }
        }
    }
    return count - LINES_SKIPPED;
}

/* Make array of groups */
student **makeGroup(int groupAmount, int studentsCount){
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

/* Read file into array of structs and count individual roles present */
int readFile(student studentList[], int rolesCount[9][2], int numberOfStudents)
{
    FILE *inFP = fopen("input.txt","r");
    char rolesStr[MAX_ROLES][4];
    int i, j, scanRes;

    if(inFP == NULL)
    {
        printf(" * Filen kunne ikke aabnes!\n");
        return 1;
    }

    if(inFP != NULL)
    {
        printf("File opended\n");

        for(i = 1; i <= LINES_SKIPPED; i++)
        {
            fscanf(inFP, " %*[^\n]\n", NULL);
        }
        for(i = 0; i < numberOfStudents; i++)
        {
            int rolesAssigned = 0;
            scanRes = fscanf(inFP, " %[^,], %d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^.].",
                             studentList[i].name, &studentList[i].ambitionLevel, rolesStr[0], rolesStr[1],
                             rolesStr[2],  studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2],
                             studentList[i].notWant);
            /*printf("%s %d %s %s %s %s %s %s %s\n", studentList[i].name, studentList[i].ambitionLevel, rolesStr[0],
                        rolesStr[1], rolesStr[2],x studentList[i].doWant[0], studentList[i].doWant[1],
                        studentList[i].doWant[2], studentList[i].notWant);*/
            if (scanRes < 9)
            {
                printf(" * Scanningsfejl i linje %d!\n", i + LINES_SKIPPED);
                return 1;
            }

            for(j = 0; j < MAX_ROLES; j++)
            {

                if(strcmp(strlwr(rolesStr[j]), "iga") == 0)
                {
                    studentList[i].roles[rolesAssigned] = iga;
                    rolesAssigned++;
                    rolesCount[iga][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "org") == 0)
                {
                    studentList[i].roles[rolesAssigned] = org;
                    rolesAssigned++;
                    rolesCount[org][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "afs") == 0)
                {
                    studentList[i].roles[rolesAssigned] = afs;
                    rolesAssigned++;
                    rolesCount[afs][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "ide") == 0)
                {
                    studentList[i].roles[rolesAssigned] = ide;
                    rolesAssigned++;
                    rolesCount[ide][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "ana") == 0)
                {
                    studentList[i].roles[rolesAssigned] = ana;
                    rolesAssigned++;
                    rolesCount[ana][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "spe") == 0)
                {
                    studentList[i].roles[rolesAssigned] = spe;
                    rolesAssigned++;
                    rolesCount[spe][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "kon") == 0)
                {
                    studentList[i].roles[rolesAssigned] = kon;
                    rolesAssigned++;
                    rolesCount[kon][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "koo") == 0)
                {
                    studentList[i].roles[rolesAssigned] = koo;
                    rolesAssigned++;
                    rolesCount[koo][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]), "for") == 0)
                {
                    studentList[i].roles[rolesAssigned] = frm;
                    rolesAssigned++;
                    rolesCount[frm][1]++;
                }
                else if(strcmp(strlwr(rolesStr[j]),"x") == 0)
                {
                    /* Nothing happens */
                }
                else
                {
                    printf(" * Fejl paa linje %d - under grupperolle #%d. Tjek bogstaver!\n", i + LINES_SKIPPED + 1, j + 1);
                }
            }
            if(rolesAssigned == 3)
            {
                printf("%s\n", studentList[i].name);
            }
            rolesAssigned = 0;
        }
        fclose(inFP);
    }
    return 0;
}

void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents)
{

    qsort(rolesCount,9 ,2*sizeof(int), rolesCmp);
    for(int i = 0; i < 9; i++)
    {
        printf("%d %d\n", rolesCount[i][0], rolesCount[i][1]);
    }
    qsort(studentList, numberOfStudents ,sizeof(student), ambitionCmp);
    for(int i = 0; i < numberOfStudents; i++)
    {
        printf("%s %d\n", studentList[i].name, studentList[i].ambitionLevel);
    }

}

void sortWishes(student studentList[], int numberOfStudents, int groupAmount, student group)
{

    /*possible shuffle before then so the first person doesnt always get all the wishes*/

    int currentAmountOfGroups = 0;
    int studentsInGroup = 0;
    int studentsPerGroup = numberOfStudents / groupAmount;
    if (studentsCount % groupAmount)
    {
        studentsPerGroup++;
    }

    student groupBuffer[studentsPerGroup];
    student conflictingStudents[10];
    int conflictingStudentsAmount = 0;

    for(int i = 0; i < numberOfStudents; i++)
    {
        if(!studentList[i].isInGroup)
        {
            groupBuffer[currentAmountOfGroups] = studentList[i];
            studentList[i].isInGroup = true;

            studentsInGroup++;

            for(int j = 0; j < 3 || studentsInGroup < studentsPerGroup; j++)
            {
                for(int k = 0; k < numberOfStudents; k++)
                {
                    if(strcmp(studentList[i].doWant[j], studentList[k].name) == 0)
                    {
                        if(!studentList[k].isInGroup)
                        {
                            groupBuffer[currentAmountOfGroups] = studentList[k];
                            studentsInGroup++;
                            studentList[k].isInGroup = true;
                        }
                    }
                }
            }

            for(int j = 0; j < studentsInGroup; j++)
            {
                group[currentAmountOfGroups][j] = groupBuffer[j];
            }

            currentAmountOfGroups++;
            studentsInGroup = 0;

        }


    }

    /*Check if someone is missing a group*/
    for(int i = 0; i < numberOfStudents; i++)
    {
        if(!studentList[i].isInGroup)
        {
            /*Check if some groups are smaller than the requested amount*/
            for(int j = 0; j < currentAmountOfGroups; j++)
            {
                for(int k = 0; k < studentsPerGroup; k++)
                {
                    else if(strcmp(group[j][k], "") == 0)
                    {
                        group[j][k] = studentList[i];
                    }
                }
            }
        }
    }

    /*Check if someone has a group with people they did not wish for*/
    for(int i = 0; i < currentAmountOfGroups; i++)
    {
        for(int j = 0; j < studentsPerGroup; j++)
        {
            for(int k = 0; k < studentsPerGroup; k++)
            {
                if(strcmp(group[i][j].doNotWant, group[i][k].name) == 0)
                {
                    conflictingStudents[conflictingStudentsAmount] = group[i][j];
                    conflictingStudentsAmount++;
                    break;
                }
            }
        }
    }

}

int rolesCmp(const void *a, const void *b)
{
    int *numa = (int*)a;
    int *numb = (int*)b;
    return (numa[1] - numb[1]);
}
int ambitionCmp(const void *a, const void *b)
{
    student *pa = (student*)a;
    student *pb = (student*)b;
    return (pb->ambitionLevel - pa->ambitionLevel);
}