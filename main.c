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
#include "readFile.h"

/******* Function declarations *******/

void belbinOrWishes(student studentList[], FILE *inFP, int rolesCount[9][2], const int studentsCount, int groupAmount, student **groups);



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

/******* Function definitions *******/


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

