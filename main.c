/*
 * Created by SW1A317b on 05/12/2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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
    role roles[3];
    char doWant[3][30];
    char notWant[30];
    bool isInGroup;
};

typedef struct student student;

/* Declare fuction prototypes */

int getLength(FILE *file);
int linesInTextfile(FILE *file);
void readFile(FILE *file, student studentList[], int lines);


int main(void)
{
	FILE *inFP = fopen("input.txt","r");
	int numStudents = getLength(inFP);
  int lines = linesInTextfile(inFP);
	student studentList[numStudents];
  readFile(inFP, studentList, lines);
  printf("%s\n", studentList[0].name);

    return 0;
}

int getLength(FILE *file)
{
    size_t position = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, position, SEEK_SET);

    return length;
}

int linesInTextfile(FILE *inFP){
    int i = 0, count = 0;
    if(inFP != NULL){

        for (i = getc(inFP); i != EOF; i = getc(inFP)){ /*Tæller linjer indtil vi rammer EOF*/

            if (i == '\n'){
                count = count + 1;
            }
        }
    }
    return count;
}

void readFile(FILE *inFP, student studentList[], int lines){
    int i = 0; /* læser filen efter guiden */
    char roleOne[4];
    char roleTwo[4];
    char roleThree[4];

    if(inFP == NULL)
    {
        printf("Couldn't open file\n");
    }

    if(inFP != NULL)
    {
      printf("File opended\n");
    }
    while (fscanf(inFP, "%*[^\n]"), i < lines){
        i++;
        printf("%d her\n", i);
        if(i > 38)
        {
          for(; i < lines; i++)
          {
            printf("%d\n", i);
            printf("hej2\n");
              fscanf(inFP, " %[^,] %d, %s, %s, %s, %[^,], %[^,], %[^,], %[^,]", studentList[i].name, &studentList[i].ambitionLevel, roleOne, roleTwo, roleThree, studentList[i].doWant[0], studentList[i].doWant[1], studentList[i].doWant[2], studentList[i].notWant);
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
