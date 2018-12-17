
#include "strlwr.h"
#include <string.h>
#include <ctype.h>

char *strlwr(char* str)
{
    int len = strlen(str);
    int i;
    for(i = 0; i < len; i++)
    {
        if(isupper((int)str[i]))
        {
            str[i] = (char)tolower((int)str[i]);
        }
    }
    return str;
}
