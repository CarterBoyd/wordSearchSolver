//
// Created by Carter Boyd on 6/29/21.
//
#include "wordSearch.h"
#include <time.h>

/**
 * Checks the word for any of the word is in the dictionary
 * @param word the word being checked for in the dictionary
 * @return 1 if the dictionary file was not found. 0 if function ran fine
 */
static void checkLine(char *word) {
    struct letterList *letterPtr;
    struct dictionary *dictPtr;
    for (letterPtr = list; letterPtr != NULL; letterPtr = letterPtr->nextLetter)
        if (letterPtr->letter == *word)
            for (dictPtr = letterPtr->list; dictPtr != NULL; dictPtr = dictPtr->next)
                if (strcmp(dictPtr->word, word) == 0) {
                    printf("%s\n", dictPtr->word);
                    return;
                }
}

/**
 * takes the line and cuts it off at length then re adds it after the line is checked
 * @param length the length of the line that is going to be cut off
 * @param line the line being cut
 * @param direction the direction the line was going
 */
void lineChange(int length, char *line) {
    char temp;
    temp = line[length];
    line[length] = '\0';
    checkLine(line);
    line[length] = temp;
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 * @return 1 if the checkline file had an error, and 0 if there were no errors
 */
static void leftToRight(char grid[width][height]) {
    char *line;
    int i = 0, j;
    printf("\nLooking for words going left to right\n");
    for (line = *grid; i < width; line += 3, ++i)
        for (; *(line + MIN_SIZE - 1); ++line)
            for (j = MIN_SIZE; *(line + j - 1); ++j)
                lineChange(j, line);
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 * @return 1 if the checkline file had an error, and 0 if there were no errors
 */
static void rightToLeft(char grid[width][height]) {
    char *line, backwards[height + 1], *ptr;
    int i, j;
    printf("\nLooking for words going right to left\n");
    for (line = *grid, i = 0; i < width; line += width + 1, ++i) {
        for (j = height; j > 0; --j)
            backwards[height - j] = line[j - 1];
        backwards[height] = '\0';
        for (ptr = backwards; *(ptr + 2); ++ptr)
            for (j = MIN_SIZE; *(ptr + j - 1); ++j)
                lineChange(j, ptr);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 * @return 1 if the checkline file had an error, and 0 if there were no errors
 */
static void topToBottom(char grid[width][height]) {
    char line[width + 1], *ptr;
    int i, j;
    printf("\nLooking for words going top to bottom\n");
    for (j = 0; j < width; ++j) {
        for (ptr = *(grid) + j, i = 0; i < height; ptr += width + 1, ++i)
            line[i] = *ptr;
        for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
            for (i = MIN_SIZE; *(ptr + i - 1); ++i)
                lineChange(i, ptr);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 * @return 1 if the checkline file had an error, and 0 if there were no errors
 */
static void bottomToTop(char grid[width][height]) {
    char line[width + 1], *ptr;
    int i, j;
    printf("\nLooking for words going bottom to top\n");
    for (j = 0; j < width; ++j) {
        for (ptr = *(grid) + j, i = height; i > 0; ptr += width + 1, --i)
            line[i - 1] = *ptr;
        for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
            for (i = MIN_SIZE; *(ptr + i); ++i)
                lineChange(i, ptr);
    }
}

/**
 * Makes sure that the file exists
 * @param pFile the file
 * @param string the name of the file
 */
static void fileExists(FILE *pFile, char *string) {
    if (pFile == NULL) {
        fprintf(stderr, "%s file not found\n", string);
        exit(EXIT_FAILURE);
    }
}

/**
 * creates the data structure that will be used to find words
 * @param link the dictionary link for the program
 */
void createDictionary(char *link) {
    int letter;
    char *line = NULL;
    size_t sz;
    FILE *ptr = fopen(link, "r");
    fileExists(ptr, link);
    struct dictionary *newDict;
    for (letter = 97; letter < 123; ++letter) {
        struct letterList *newLetter = malloc(sizeof(struct letterList));
        newLetter->letter = (char) letter;
        newLetter->nextLetter = NULL;
        newLetter->list = NULL;
        if (line != NULL) {
            newDict = malloc(sizeof(struct dictionary));
            newDict->word = strdup(line);
            newDict->next = newLetter->list;
            newLetter->list = newDict;
        }
        while (getline(&line, &sz, ptr) != EOF) {
            if (*line >= 65 && *line <= 90)
                *line += 32;
            line[strlen(line) - 1] = '\0';
            if (*line != (char) letter)
                break;
            newDict = malloc(sizeof(struct dictionary));
            newDict->word = strdup(line);
            if (newLetter->list == NULL) {
                newLetter->list = newDict;
            } else {
                newDict->next = newLetter->list;
                newLetter->list = newDict;
            }
        }
        if (letter == 97)
            list = newLetter;
        else {
            struct letterList *letterPtr = list;
            while (letterPtr->nextLetter != NULL)
                letterPtr = letterPtr->nextLetter;
            letterPtr->nextLetter = newLetter;
        }
    }
    fclose(ptr);
}

/**
 * Frees all the malloced memory
 * @param piss the dictionary
 */
static void freeAll() {
    struct letterList *next;
    struct dictionary *linkedListNext, *linkedListTemp;
    while (list != NULL) {
        next = list->nextLetter;
        linkedListNext = list->list;
        while (linkedListNext != NULL) {
            linkedListTemp = linkedListNext->next;
            free(linkedListNext->word);
            free(linkedListNext);
            linkedListNext = linkedListTemp;
        }
        free(list);
        list = next;
    }
}

int main(int argc, char *argv[]) {
    clock_t begin = clock();

    int i, j;
    if (argc - 1 == 2) {
        createDictionary(argv[2]);
        FILE *gridFile;
        gridFile = fopen(argv[1], "r");
        fileExists(gridFile, argv[1]);
        fscanf(gridFile, "%d %d", &width, &height);
        char grid[width + 1][height + 1], letter[3];
        fscanf(gridFile, "%c", letter);
        for (i = 0; i < width; ++i) {
            for (j = 0; j < height; ++j) {
                fscanf(gridFile, "%2s", letter);
                grid[i][j] = *letter;
            }
            grid[i][height] = '\0';
        }
        grid[width][0] = '\0';
        fclose(gridFile);
        leftToRight(grid);
        rightToLeft(grid);
        topToBottom(grid);
        bottomToTop(grid);
        freeAll();

        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("%f", time_spent);
        return EXIT_SUCCESS;
    }
    fprintf(stdout, "requires 2 arguments, there are %d\n", argc - 1);
    return EXIT_FAILURE;
}