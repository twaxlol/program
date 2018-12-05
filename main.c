//
// Created by Frederik on 05-12-2018.
//

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
