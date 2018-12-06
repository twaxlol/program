/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROLES 3
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

int getStudents(FILE *file);
int linesInTextfile(FILE *file);
void readFile(FILE *inFP, student studentList[], int rolesCount[9][2], int lines);

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
    int numStudents = getStudents(inFP);
    int lines = linesInTextfile(inFP);
    student studentList[numStudents];
    readFile(inFP, studentList, lines, rolesCount);
    printf("%s\n", studentList[0].name);

    return 0;
}

int getStudents(FILE *file) /* Skal ændres til kun at tælle linjer med studerendes data */
{
    size_t position = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, position, SEEK_SET);

    return length;
}

int linesInTextfile(FILE *inFP){
    int i = 0, count = 0;
    if(inFP != NULL)
    {
        for (i = getc(inFP); i != EOF; i = getc(inFP)) /*Tæller linjer indtil vi rammer EOF*/
        {
            if (i == '\n'){
                count = count + 1;
            }
        }
    }
    return count;
}

void readFile(FILE *inFP, student studentList[], int rolesCount[9][2], int lines)
{
    int i = 0; /* læser filen efter guiden */

    if(inFP == NULL)
    {
        printf("Couldn't open file\n");
    }

    if(inFP != NULL)
    {
        printf("File opended\n");
    }
    while (fscanf(inFP, "%*[^\n]"), i < lines)
    {
        i++;
        printf("%d her\n", i);
        char rolesStr[MAX_ROLES][4];
        if(i > 38)
        {
            for(; i < lines; i++)
            {
                int rolesAssigned = 0;
                printf("%d\n", i);
                printf("hej2\n");
                fscanf(inFP, " %[^,] %d, %s, %s, %s, %[^,], %[^,], %[^,], %[^,]", studentList[i].name, &studentList[i].ambitionLevel, rolesStr[0], rolesStr[1], rolesStr[2], studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2], studentList[i].notWant);
                /* Navn, Ambitionsniveau, Rolle 1, Rolle 2, Rolle 3, Ønske 1, Ønske 2, Ønske 3, Fravalg */

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
                        printf("Fejl på linje %d - under grupperoller. Check bogstaverne", i + 1);
                        getchar();
                    }
                }
                rolesAssigned = 0;

                printf("%s ", studentList[i].name);
            }
        }
    }
}

//     for(i; i < lines; i++)
//     {
//       printf("%d\n", i);
//       printf("hej2\n");
//         fscanf(inFP, " %[^,] %d, %s, %s, %s, %[^,], %[^,], %[^,], %[^,].",
//                 studentList[i].name, &studentList[i].ambitionLevel, roleOne, roleTwo, roleThree,
//                 studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2], studentList[i].notWant);
//         printf("%s ", studentList[i].name);
//     }
//     }
// }
