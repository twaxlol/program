/*
 * Created by SW1A317b on 05/12/2018
 */

FILE *inFP;

enum roles
{
    iga,
    org,
    afs,
    ide,
    ana,
    spe,
    kon,
    koo,
    frm
};

typedef struct student
{
    char name[50];
    int ambitionLevel; //from 1 to 5
    enum roles roleSet[3];
    char doWant[3];
    char notWant;

} student;


void main()
{
    struct student studentList[getLength(inFP)];
}

int getLength(FILE *file)
{
    //Find hvor mange elever der er
}
