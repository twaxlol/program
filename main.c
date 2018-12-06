/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROLES 3
#define SKIP_LINES 38
/* Define enum and structs */
enum role
{
    iga, /* Igangsætter */
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

int numberOfStudents(FILE *file);
void readFile(student studentList[], int rolesCount[9][2], int lines);

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
    int studentsCount = numberOfStudents(inFP);
    student studentList[studentsCount];
    readFile(studentList, rolesCount, studentsCount);

    return 0;
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
    return count - SKIP_LINES;
}

void readFile(student studentList[], int rolesCount[9][2], int numberOfStudents)
{
    FILE *inFP = fopen("input.txt","r");
    char rolesStr[MAX_ROLES][4];


    if(inFP == NULL)
    {
        printf("Couldn't open file\n");
        exit(1);
    }

    if(inFP != NULL)
    {
        printf("File opended\n");
    }
    for(int i = 1; i <= SKIP_LINES; i++){
        fscanf(inFP, " %*[^\n]\n", NULL);
    }
    for(int i = 0; i < numberOfStudents; i++)
    {
        int rolesAssigned = 0;
        fscanf(inFP, " %[^,], %d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^.].",
                studentList[i].name, &studentList[i].ambitionLevel, rolesStr[0], rolesStr[1],
                rolesStr[2],  studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2],
                studentList[i].notWant);
        /*printf("%s %d %s %s %s %s %s %s %s\n", studentList[i].name, studentList[i].ambitionLevel, rolesStr[0],
                    rolesStr[1], rolesStr[2],x studentList[i].doWant[0], studentList[i].doWant[1],
                    studentList[i].doWant[2], studentList[i].notWant);*/
        for(int j = 0; j < MAX_ROLES; j++)
        {

            if(strcmp(strlwr(rolesStr[j]), "iga") == 0)
            {
                studentList[i].roles[rolesAssigned] = iga;
                rolesAssigned++;
                rolesCount[0][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "org") == 0)
            {
                studentList[i].roles[rolesAssigned] = org;
                rolesAssigned++;
                rolesCount[1][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "afs") == 0)
            {
                studentList[i].roles[rolesAssigned] = afs;
                rolesAssigned++;
                rolesCount[2][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "ide") == 0)
            {
                studentList[i].roles[rolesAssigned] = ide;
                rolesAssigned++;
                rolesCount[3][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "ana") == 0)
            {
                studentList[i].roles[rolesAssigned] = ana;
                rolesAssigned++;
                rolesCount[4][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "spe") == 0)
            {
                studentList[i].roles[rolesAssigned] = spe;
                rolesAssigned++;
                rolesCount[5][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "kon") == 0)
            {
                studentList[i].roles[rolesAssigned] = kon;
                rolesAssigned++;
                rolesCount[6][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "koo") == 0)
            {
                studentList[i].roles[rolesAssigned] = koo;
                rolesAssigned++;
                rolesCount[7][0]++;
            }
            else if(strcmp(strlwr(rolesStr[j]), "for") == 0)
            {
                studentList[i].roles[rolesAssigned] = frm;
                rolesAssigned++;
                rolesCount[8][0]++;
            }
            else
            {
                printf("Fejl paa linje %d - under grupperoller. Check bogstaverne\n", i + 39);
            }
        }
        printf("%s\n", studentList[i].name);
        rolesAssigned = 0;
    }
    fclose(inFP);
}
