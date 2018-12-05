/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
/* Define enum and structs */
enum roles
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
typedef enum roles roles;

struct student
{
    char name[30];
    int ambitionLevel; /*from 1 to 5*/
    enum roles roleSet[3];
    char doWant[3][30];
    char notWant[30];

};

typedef struct student student;

/* Declare fuction prototypes */

int getLength(FILE *file);

int main()
{
	FILE *inFP;
	student studentList[getLength(inFP)];

    return 0;
}

int getLength(FILE *file)
{
    /*Find hvor mange elever der er*/
}
