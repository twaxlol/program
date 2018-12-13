/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "strlwr.h"

/* Global values */
#define MAX_ROLES 3
#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

/* Define enum and structs */
enum role
{
    err = -1, /*Error*/
    non = 0,  /* No role */
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

/******* Function declarations *******/
int numberOfStudents(FILE *file);
int getGroupCount(FILE* inFP);
int readFile(student studentList[], int rolesCount[9][2], const int numberOfStudents);
student **makeGroup(const int groupAmount, const int studentsCount);
sort getMode(FILE* inFP);
sort callSortMode(FILE *inFP, int groupAmount);
void belbinOrWishes(student studentList[], sort sortMode, int rolesCount[9][2], const int studentsCount, int groupAmount, student **groups);
role strToRole(const char *inStr);
void sortBelbin(student studentList[], int rolesCount[9][2], int numberOfStudents, int groupAmount, student **groups);
bool studentHasRole(const role inRole, const student *inStudent);
bool groupMissingRole(const student group[], const role inRole, const int groupSize);
void addToGroup(student group[], student *inStudent, const int groupSize);
int studentsInGroup(const student group[], const int groupSize);
int findBestGroup( const student *inStudent, student **groups, const int groupAmount, const int groupSize);
double averageAmbitionInGroup(const student group[], const int groupSize );
int rolesCmp(const void *a, const void *b);
int ambitionCmp(const void *a, const void *b);

/******* Main function *******/
int main(void)
{
    int rolesCount[9][2] = {{iga, 0}, {org, 0}, {afs, 0},
                            {ide, 0}, {ana, 0}, {spe, 0},
                            {kon, 0}, {koo, 0}, {frm, 0}};

    /* variables used for numerous functions */
    FILE *inFP = fopen("input.txt","r");
    int studentsCount = numberOfStudents(inFP);
    int groupAmount = getGroupCount(inFP);

    /* Makes struct array of students and SOMETHING HERE */
    student studentList[studentsCount];
    student **groups = makeGroup(groupAmount, studentsCount);

    /* Chooses which way to make groups */
    sort sortMode = error;
    sortMode = callSortMode(inFP, groupAmount);
    belbinOrWishes(studentList, sortMode, rolesCount, studentsCount, groupAmount, groups);

    return 0;
}

/******* Function calls *******/
/*Input:  Textfile with students*/
/*Do      Checks the wanted amount of groups from line 25, in textfile*/
/*Output: Number of groups*/
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

/*Input:  The file pointer, the amount of groups and which sorting mode.*/
/*Do      Checks if amount of groups and sorting mode is correctly set in the input file */
/*Output: Returns the sorting mode. If everything in the input file is okay,
          then it returns the mode, else it returns an error. */
sort callSortMode(FILE *inFP, int groupAmount)
{
    sort sortMode = getMode(inFP);
    if (groupAmount != 0 || sortMode != error)
    {
        return sortMode = getMode(inFP);
        rewind(inFP);
    }
    return sortMode = error;
}

/* Input:  Struct array of students, sorting mode, amount of each role, and amount of students */
/* Do:     Chooses sorting mode and calls the chosen function */
/* Output: sortBelbin or makeWishGroups function */
void belbinOrWishes(student studentList[], sort sortMode, int rolesCount[9][2],
                    const int studentsCount, int groupAmount, student **groups)
{
    int state = readFile(studentList, rolesCount, studentsCount);
    if (state != 0)
    {
        exit(1);
    }
    if(sortMode == belbin)
    {
        sortBelbin(studentList, rolesCount, studentsCount, groupAmount, groups);
    }
    else if(sortMode == wish)
    {
        /*makeWishGroups();*/
    }
    else
    {
        printf("FEJL prøv igen :)\n");
    }
}

/*Input:  Textfile with students*/
/*Do:     Check desired mode (Belbin or Wish) from input file */
/*Output: Returns which mode is wanted*/
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

/*Input:  Textfile with students, amount of roles & amouts of students with said role, number of students */
/*Do      Copy students from input file to array of structs and count individual group roles present */
/*Output: Necessary student information, has been stored*/
int readFile(student studentList[],  int rolesCount[9][2], const int numberOfStudents)
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
        for(i = 1; i <= LINES_SKIPPED; i++)
        {
            fscanf(inFP, " %*[^\n]\n");
        }
        for(i = 0; i < numberOfStudents; i++)
        {
            int rolesAssigned = 0;
            scanRes = fscanf(inFP, " %[^,], %d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^.].",
                            studentList[i].name, &studentList[i].ambitionLevel, rolesStr[0], rolesStr[1],
                            rolesStr[2],  studentList[i].doWant[0], studentList[i].doWant[1],
                            studentList[i].doWant[2], studentList[i].notWant);
            if (scanRes < 9)
            {
                printf(" * Scanningsfejl i linje %d!\n", i + LINES_SKIPPED);
                return 1;
            }
            for(j = 0; j < MAX_ROLES; j++)
            {
                role inRole = strToRole(strlwr(rolesStr[j]));
                if(inRole > 0)
                {
                    studentList[i].roles[rolesAssigned] = inRole;
                    rolesCount[inRole-1][1]++;
                    rolesAssigned++;
                    printf("%s %d\n", studentList[i].name, rolesAssigned);
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
            rolesAssigned = 0;
        }
        fclose(inFP);
    }
    return 0;
}

/* Input:  The 3 Belbin roles as rolesStr[] from readFile function */
/* Do:     Checks which Belbin role is read from the user textfile. */
/* Output: Returns the enum value for the Belbin role. */
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
        return non;
    }
    else
    {
        return err;
    }
}

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

int studentsInGroup(const student group[], const int groupSize)
{
    int i = 0;
    while(i < groupSize && strcmp(group[i].name,""))
    {
        i++;
    }
    return i;
}

int findBestGroup( const student *inStudent, student **groups, const int groupAmount, const int groupSize)
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

double averageAmbitionInGroup(const student group[], const int groupSize )
{
    int i,res = 0;
    for(i = 0; i < groupSize; i++)
    {
        res += group[i].ambitionLevel;
    }
    return (double)res/(double)groupSize;
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
