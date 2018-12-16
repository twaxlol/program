/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "strlwr.h"
#include "belbinRoles.h"
#include "studentsGroups.h"

/* Global values */
#define MAX_ROLES 3
#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

/* Define enum and structs */


enum sort
{
    error = 0,
    belbin,
    wish
};
typedef enum sort sort;


/******* Function declarations *******/

int readFile(student studentList[], int rolesCount[9][2], const int numberOfStudents);
void makeNewInput(FILE *newIn);
sort getMode(FILE* inFP);
void belbinOrWishes(student studentList[], FILE *inFP, int rolesCount[9][2], const int studentsCount, int groupAmount, student **groups);
role strToRole(const char *inStr);


/******* Main function *******/
int main(void)
{
    int rolesCount[9][2] = {{iga, 0}, {org, 0}, {afs, 0},
                            {ide, 0}, {ana, 0}, {spe, 0},
                            {kon, 0}, {koo, 0}, {frm, 0}};

    FILE *inFP = fopen("input.txt","r");

    FILE *outFP = freopen("output.txt","w",stdout);
    if(inFP == NULL)
    {
        printf(" * Filen kunne ikke aabnes!\n"
               "Laver ny input skabelon!\n");
        FILE *newIn = fopen("input.txt", "w");
        makeNewInput(newIn);

    }
    else {

        /* variables used for numerous functions */
        int studentsCount = numberOfStudents(inFP);
        printf("%d\n", studentsCount);
        int groupAmount = getGroupCount(inFP);

        /* Makes struct array of students and SOMETHING HERE */
        student studentList[studentsCount];
        student **groups = makeGroup(groupAmount, studentsCount);

        int state = readFile(studentList, rolesCount, studentsCount);
        if (state != 0)
        {
            exit(1);
        }
        /* Chooses which way to make groups */
        belbinOrWishes(studentList, inFP, rolesCount, studentsCount, groupAmount, groups);
        /* Print made groups */
        printGroups(groups, groupAmount,(studentsCount/groupAmount)+1);

        /* Close file streams */
        fclose(inFP);
        fclose(outFP);
    }

    return 0;
}

/******* Function calls *******/


/* Input:  Pointer to inputfile stream, array of amount of roles, amount of students, amount of groups*/
/* Do:     Chooses sorting mode and calls the chosen function, sortung the students into groups */
/* Output:  array of groups*/
void belbinOrWishes(student studentList[], FILE *inFP, int rolesCount[9][2],
                    const int studentsCount, int groupAmount, student **groups)
{

    sort sortMode = getMode(inFP);
    if (groupAmount != 0 || sortMode != error)
    {
        sortMode = getMode(inFP);
        rewind(inFP);
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

/*Input:  Indput file pointer */
/*Do:     Reads desired mode (Belbin or Wish) from input file */
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


/*Input:  Number of students */
/*Do      Copy students from input file to array of structs and count individual group roles present */
/*Output: Necessary student information, has been stored, array of students, array of amount of roles*/
int readFile(student studentList[],  int rolesCount[9][2], const int numberOfStudents)
{
    FILE *inFP = fopen("input.txt","r");
    char rolesStr[MAX_ROLES][4];
    int i, j, scanRes;

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
        studentList[i].isInGroup = false;
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

    return 0;
}

/* Input:  File pointer */
/* Do:     Write an input template to file   */
/* Output: No output   */
void makeNewInput(FILE *newIn)
{
    fprintf(newIn,"#=====================================================================================\n"
                  "# Ambitionsniveau angives på skala fra 1 til 5\n"
                  "#=====================================================================================\n"
                  "#   1: Meget lavt\n"
                  "#   2: Lavt\n"
                  "#   3: Middel\n"
                  "#   4: Højt\n"
                  "#   5: Meget højt\n"
                  "#=====================================================================================\n"
                  "# Grupperoller angives som de første tre bogstaver\n"
                  "#=====================================================================================\n"
                  "# Handletyper   | Igangsætter:\t iga\n"
                  "#               | Organisator:\t org\n"
                  "#               | Afslutter:     afs\n"
                  "# Tænketyper    | Idéskaber:     ide\n"
                  "#               | Analysator:    ana\n"
                  "#               | Specialist:    spe\n"
                  "# Sociale typer | Kontaktskaber: kon\n"
                  "#               | Koordinator:   koo\n"
                  "#               | Formidler:     for\n"
                  "#=====================================================================================\n"
                  "# Hvordan skal grupperne dannes?\n"
                  "#=====================================================================================\n"
                  "# Antal grupper der skal dannes:\n"
                  "#   Grupper: [ 3 ]\n"
                  "#\n"
                  "# Gruppernes fokus (SÆT ÉT KRYDS (x)):\n"
                  "#   Fagligt: [ X ]\n"
                  "#   Socialt: [  ]\n"
                  "#=====================================================================================\n"
                  "# Elevdata skrives i bunden af dokumentet på følgende form\n"
                  "#=====================================================================================\n"
                  "Navn, Ambitionsniveau, Rolle 1, Rolle 2, Rolle 3, Ønske 1, Ønske 2, Ønske 3, Fravalg.\n"
                  "# F.eks.:\n"
                  "Peter Jensen, 2, iga, ide, kon, , , , Anne Nielsen.\n"
                  "Anne Nielsen, 4, org, ana, koo, Peter Jensen, , , .\n"
                  "...\n"
                  "#=====================================================================================");
    fclose(newIn);
}

/* Input:  A string containing the name of a group role */
/* Do:     Checks which Belbin role is contained in the string */
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

