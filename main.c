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
void readFile(student studentList[], int rolesCount[9][2], int lines);
void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents, int groupAmount, student **groups);
int rolesCmp(const void *a, const void *b);
int ambitionCmp(const void *a, const void *b);



int main(void)
{
    /*index 0 er grupperollen. Index 1 er antallet af grupperoller i klassen*/
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

    readFile(studentList, rolesCount, studentsCount);

    sortBelbin(studentList, rolesCount, studentsCount, groupAmount, groups);

    if( sortMode == belbin)
    {
        /*makeBelbinGroups();*/
        sortBelbin(studentList, rolesCount, studentsCount, groupAmount, groups);
    }
    else if( sortMode == wish)
    {
        /*makeWishedGroups();*/
    }
    else{
        printf("FEJL prøv igen :)\n");
    }




    return 0;
}

int getGroupCount(FILE* inFP)
{
    int i, groupAmount = 0, scanRes;
    for(i = 0; i < 24; i++)
    {
        fscanf(inFP, "%*[^\n]");
    }
    scanRes = fscanf(inFP, "%*[^[] [ %d %*[^\n]", &groupAmount);
    if (scanRes == 0)
    {
        printf(" * Fejl i linje 25 - gruppeantal. Husk at skrive oensket antal grupper!\n");
    }
    else if (groupAmount == 0)
    {
        printf(" * Fejl i linje 25 - gruppeantal. Gruppeantal kan ikke vaere 0!\n");
    }
    return groupAmount;
}

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

void readFile(student studentList[], int rolesCount[9][2], int numberOfStudents)
{
    FILE *inFP = fopen("input.txt","r");
    char rolesStr[MAX_ROLES][4];
    int i, j;

    if(inFP == NULL)
    {
        printf("Couldn't open file\n");
        exit(1);
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
            fscanf(inFP, " %[^,], %d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^.].",
                    studentList[i].name, &studentList[i].ambitionLevel, rolesStr[0], rolesStr[1],
                    rolesStr[2],  studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2],
                    studentList[i].notWant);
            studentList[i].isInGroup = false;
            /*printf("%s %d %s %s %s %s %s %s %s\n", studentList[i].name, studentList[i].ambitionLevel, rolesStr[0],
                        rolesStr[1], rolesStr[2],x studentList[i].doWant[0], studentList[i].doWant[1],
                        studentList[i].doWant[2], studentList[i].notWant);*/
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
}

void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents, int groupAmount, student **groups)
{
    int j = 0;
    qsort(rolesCount,9 ,2*sizeof(int),rolesCmp);
    qsort(studentList, numberOfStudents ,sizeof(student), ambitionCmp);
    for(int i = 0; i < 9; i++)
    {
        printf("%d %d\n", rolesCount[i][0], rolesCount[i][1]);
    }
    for(int i = 0; i < numberOfStudents; i++)
    {
        printf("%s %d\n", studentList[i].name, studentList[i].ambitionLevel);
    }
    for (int i = 0; i < numberOfStudents; i++)
    {
        if(rolesCount[0][0] == studentList[i].roles[0] || rolesCount[0][0] == studentList[i].roles[1] || rolesCount[0][0] == studentList[i].roles[2])
        {
            if(studentList[i].isInGroup == false)
            {
                groups[j][0] = studentList[i];
                j++;
                studentList[i].isInGroup = true;
            }
        }


    }
    for (int i = 0; i < groupAmount; i++)
    {
        printf("group %d: %s\n", i , groups[i][0].name);
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
