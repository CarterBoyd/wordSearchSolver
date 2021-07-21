//
// Created by Carter Boyd on 7/7/21.
//

#ifndef WORDSEARCH_WORDSEARCH_H
#define WORDSEARCH_WORDSEARCH_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MIN_SIZE 3
#define UPPERCASE_BEGIN 65
#define UPPERCASE_END 90
#define ALPHABET_SIZE 26
#define TOLOWER_CASE 32
#define ASCII_LOWERCASE 97

struct dictionary {
    char *word;
    struct dictionary *next;
};

struct dictionary *list[26];
int width, height;

#endif //WORDSEARCH_WORDSEARCH_H
