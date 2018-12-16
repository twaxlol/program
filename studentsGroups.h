//
// Created by adrian plesner on 14/12/2018.
//

#ifndef PROGRAM_STUDENTSGROUPS_H
#define PROGRAM_STUDENTSGROUPS_H


#define MAX_ROLES 3
#define LINES_SKIPPED 38
#define UI_LINES_SKIPPED 25

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

student **makeGroup(const int groupAmount, const int studentsCount);
void addToGroup(student group[], student *inStudent, const int groupSize);
int studentsInGroup(const student group[], const int groupSize);
double averageAmbitionInGroup(const student group[], const int groupSize );
void printGroups(student **groups, const int groupAmount, const int );
void roleToStr(const role inRole, char str[]);

#endif //PROGRAM_STUDENTSGROUPS_H
