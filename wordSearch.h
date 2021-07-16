//
// Created by Carter Boyd on 7/7/21.
//

#ifndef WORDSEARCH_WORDSEARCH_H
#define WORDSEARCH_WORDSEARCH_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct dictionary {
    char *word;
    struct dictionary *next;
};

struct letterList {
    char letter;
    struct letterList *nextLetter;
    struct dictionary *list;
};

struct letterList *list = NULL;
const static int MIN_SIZE = 3;
int width, height;

#endif //WORDSEARCH_WORDSEARCH_H
