
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "groups.h"
#include "readFile.h"
#include "strlwr.h"

/*Input:  File pointer*/
/*Do:     Reads the desired amount of groups from the file*/
/*Output: Returns amount of groups*/
int getGroupCount(FILE* inFP, const int studentsCount)
{
    int i, groupAmount = 0, scanRes;

    for(i = 0; i <= UI_LINES_SKIPPED; i++)
    {
        fscanf(inFP, "%*[^\n]");
    }
    scanRes = fscanf(inFP, " %*[^[] [ %d %*[^\n]", &groupAmount);
    if (scanRes == 0)
    {
        printf(" * Fejl i linje %d - gruppeantal! Husk at skrive oensket antal grupper!\n", UI_LINES_SKIPPED);
        exit(1);
    }
    else if (groupAmount < 3)
    {
        printf(" * Fejl i linje %d - gruppeantal! Gruppeantal skal være mindst 3!\n", UI_LINES_SKIPPED);
        exit(1);
    }
    else if (groupAmount > studentsCount / 2 - 1)
    {
        printf(" * Fejl i linje %d - gruppeantal! Gruppeantal for hoejt i forhold til antal elever!\n", UI_LINES_SKIPPED);
        exit(1);
    }

    return groupAmount;
}

/*Input:  File pointer*/
/*Do:     Find number of students (newlines) from input file*/
/*Output: Returns number of students*/
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
    return (count - LINES_SKIPPED + 1);
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
    printf(" * Fejl i linje 28/29 - prioritetsmode! Saet et enkelt kryds (x)!\n");
    return error;
}

/*Input:  Number of students */
/*Do      Copy students from input file to array of structs and count individual group roles present */
/*Output: Necessary student information, has been stored, array of students, array of amount of roles*/
int readFile(student studentList[],  int rolesCount[9][2], const int numberOfStudents)
{
    FILE *inFP = fopen("input.txt","r");
    char rolesStr[MAX_ROLES][4];
    int i, j, scanRes, errState = 0;

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

        if(studentList[i].ambitionLevel > 5 || studentList[i].ambitionLevel < 1)
        {
            printf(" * Fejl i linje %d! Det indtastede ambitionsniveau er ikke mellem 1 og 5!\n", i + LINES_SKIPPED + 1);
            errState++;
        }
        else if (scanRes < 9)
        {
            printf(" * Fejl i linje %d! Stemmer indtastede parametre overens med oenskede format?\n", i + LINES_SKIPPED + 1);
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
                printf(" * Fejl i linje %d - grupperolle #%d! Stemmer grupperolle overens med forkortelse?\n", i + LINES_SKIPPED + 1, j + 1);
                errState++;
            }
        }
        rolesAssigned = 0;
    }
    if(errState > 0)
    {
        return 1;
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
                  "# Grupperoller angives som de første tre bogstaver (hvis intet, skriv \"x\")\n"
                  "#=====================================================================================\n"
                  "# Handletyper---|-Igangsætter:\t iga\n"
                  "#               | Organisator:\t org\n"
                  "#               | Afslutter:     afs\n"
                  "# Tænketyper----|-Idéskaber:     ide\n"
                  "#               | Analysator:    ana\n"
                  "#               | Specialist:    spe\n"
                  "# Sociale typer-|-Kontaktskaber: kon\n"
                  "#               | Koordinator:   koo\n"
                  "#               | Formidler:     for\n"
                  "#=====================================================================================\n"
                  "# Hvordan skal grupperne dannes?\n"
                  "#=====================================================================================\n"
                  "# Antal grupper der skal dannes:\n"
                  "#   Grupper: [  ]\n"
                  "#\n"
                  "# Gruppernes fokus (SÆT ÉT KRYDS (x)):\n"
                  "#   Fagligt: [  ]\t\t\t\t    (Roller og ambition som prioritet)\n"
                  "#   Socialt: [  ]\t\t\t\t    (Ønsker og ambition som prioritet)\n"
                  "#=====================================================================================\n"
                  "# Elevdata skrives i bunden af dokumentet på følgende form\n"
                  "#=====================================================================================\n"
                  "Navn, Ambitionsniveau, Rolle 1, Rolle 2, Rolle 3, Ønske 1, Ønske 2, Ønske 3, Fravalg.\n"
                  "# F.eks.:\n"
                  "Peter Jensen, 2, iga, ide, kon, Anders Bo, Rune Pedersen, Karoline Skov, Anne Nielsen.\n"
                  "Anne Nielsen, 4, org, ana, koo, Peter Jensen, Karoline Skov, Anders Bo, Rune Pedersen.\n"
                  "...\n"
                  "#=================================|ELEVDATA HERUNDER|=================================\n");
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
