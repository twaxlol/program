//
// Created by adrian plesner on 07/12/2018.
//
#include "strlwr.h"
#include <string.h>
#include <ctype.h>

char *strlwr(char* str)
{
    int len = strlen(str);
    for(int i = 0; i < len; i++)
    {
        if(isupper((int)str[i]))
        {
            str[i] = (char)tolower((int)str[i]);
        }
    }
    return str;
}
