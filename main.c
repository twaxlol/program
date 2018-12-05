/*
 * Created by SW1A317b on 05/12/2018
 */

FILE *inFP;

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

typedef struct student
{
    char name[35];
    int ambitionLevel; //from 1 to 5
    enum roles roleSet[3];
    char doWant[3][30];
    char notWant[30];

} student;


void main()
{
    struct student studentList[getLength(inFP)];
}

int getLength(FILE *file)
{
    //Find hvor mange elever der er
}
