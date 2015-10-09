/***********************************************************************
*                                                                      *
*                                                *
*                                                                      *
***********************************************************************/

#include "def.h"
#include <stdio.h>

int isPrefix(char *Key, char *Word) {

    int lenK = strlen(Key);
    int lenW = strlen(Word);
    if(lenK > lenW) return  0;

    for(int i = 0; i < lenK; i++){
        if(Key[i] != Word[i]){
             return 0;
        }
    }
    return 1;
}
