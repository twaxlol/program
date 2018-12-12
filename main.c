/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "strlwr.h"
/* Global values */
#define MAX_ROLES 3
#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

/* Define enum and structs */
enum role
{
    err = -1, /*Error*/
    nan = 0,  /* No role */
    iga = 1, /* Igangsætter */
    org, /* Organisator */
    afs, /* Afslutter */
    ide, /* Ideskaber */
    ana, /* Analysator */
    spe, /* Specialist */
    kon, /* Kontaktskaber */
    koo, /* Koordinator */
    frm  /* Formidler */
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
role strToRole(const char *inStr);
void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents);
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
        /*makeWishGroups();*/
    }
    else{
        printf("FEJL prøv igen :)\n");
    }
    return 0;
}

/* Check desired amount of groups from input file */
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

/* Check desired mode (Belbin or Wish) from input file */
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

/* Find number of students (newlines) from input file*/
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

/* Create array of groups with size determined by number of students and amount of groups */
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

/* Copy students from input file to array of structs and count individual group roles present */
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
                role inRole = strToRole(strlwr(rolesStr[j]));
                if(inRole > 0) {
                    studentList[i].roles[rolesAssigned] = inRole;
                    rolesCount[inRole][1]++;
                    rolesAssigned++;
                }
                else if(inRole == 0)
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

role strToRole(const char *inStr)
{
    if(strcmp(inStr, "iga") == 0)
    {
        return iga;
    }
    else if(strcmp(inStr, "org") == 0)
    {
        return org;
    }
    else if(strcmp(inStr, "afs") == 0)
    {
        return afs;
    }
    else if(strcmp(inStr, "ide") == 0)
    {
        return ide;
    }
    else if(strcmp(inStr, "ana") == 0)
    {
        return ana;
    }
    else if(strcmp(inStr, "spe") == 0)
    {
        return spe;
    }
    else if(strcmp(inStr, "kon") == 0)
    {
        return kon;
    }
    else if(strcmp(inStr, "koo") == 0)
    {
        return koo;
    }
    else if(strcmp(inStr, "for") == 0)
    {
        return frm;
    }
    else if(strcmp(inStr,"x") == 0 )
    {
        return nan;
    }
    else
    {
        return err;
    }

}

/* Sort groups using Belbin roles and ambition as main factors */
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

/* Compare Belbin roles */
int rolesCmp(const void *a, const void *b)
{
    int *numa = (int*)a;
    int *numb = (int*)b;
    return (numa[1] - numb[1]);
}

/* Compare ambition levels */
int ambitionCmp(const void *a, const void *b)
{
    student *pa = (student*)a;
    student *pb = (student*)b;
    return (pb->ambitionLevel - pa->ambitionLevel);
}
